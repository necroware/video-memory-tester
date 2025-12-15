// Necrowares's Video Memory Tester
// Copyright (C) 2025 by Necroware
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "vbe.hpp"

#include <dpmi.h>
#include <dpmi.hpp>
#include <go32.h>
#include <sys/farptr.h>

#include <cstring>
#include <stdexcept>

namespace vbe {

namespace internal {

class error : public std::runtime_error {
public:
    explicit error(const std::string& msg)
    : std::runtime_error{"vbe: " + msg} {}
};

struct __attribute__((packed)) controller_info_t {
    std::uint8_t signature[4];
    std::uint16_t version;
    dpmi::farptr_t oem_string_ptr;
    std::uint8_t capabilities[4];
    dpmi::farptr_t video_mode_ptr;
    std::uint16_t total_memory;
    std::uint16_t oem_software_rev;
    dpmi::farptr_t oem_vendor_name_ptr;
    dpmi::farptr_t oem_product_name_ptr;
    dpmi::farptr_t oem_product_rev_ptr;
    std::uint8_t reserved[222];
    std::uint8_t oem_data[256];
};
static_assert(sizeof(controller_info_t) == 512);

struct __attribute__((packed)) mode_info_t {
    std::uint16_t mode_attributes;
    std::uint8_t win_a_attributes;
    std::uint8_t win_b_attributes;
    std::uint16_t win_granularity;
    std::uint16_t win_size;
    std::uint16_t win_a_segment;
    std::uint16_t win_b_segment;
    std::uint32_t win_func_ptr;
    std::uint16_t bytes_per_scanline;
    std::uint16_t x_resolution;
    std::uint16_t y_resolution;
    std::uint8_t x_char_size;
    std::uint8_t y_char_size;
    std::uint8_t number_of_planes;
    std::uint8_t bits_per_pixel;
    std::uint8_t number_of_banks;
    std::uint8_t memory_model;
    std::uint8_t bank_size;
    std::uint8_t number_of_image_pages;
    std::uint8_t reserved1;
    std::uint8_t red_mask_size;
    std::uint8_t red_field_position;
    std::uint8_t green_mask_size;
    std::uint8_t green_field_position;
    std::uint8_t blue_mask_size;
    std::uint8_t blue_field_position;
    std::uint8_t reserved_mask_size;
    std::uint8_t reserved_field_position;
    std::uint8_t direct_color_mode_info;
    std::uint32_t phys_base_ptr;
    std::uint32_t offscreen_mem_offset;
    std::uint16_t offscreen_mem_size;
    std::uint8_t reserved2[206];
};

void call(__dpmi_regs r) {
    if (__dpmi_int(0x10, &r) != 0 || r.x.ax != 0x004F) {
        throw error("Failed to get VESA info");
    }
}

auto get_controller_info() -> const controller_info_t& {
    static const auto result = [] {
        auto info = dpmi::typed_dos_memory<controller_info_t>{};
        std::memcpy(info->signature, "VBE2", 4);
        info.push();
        __dpmi_regs r{};
        r.x.ax = 0x4F00;
        r.x.es = info.segment();
        call(r);
        info.pull();
        if (std::memcmp(info->signature, "VESA", 4) != 0) {
            throw error("Invalid VESA info returned");
        }
        if (info->version < 0x200) {
            throw error("Invalid VESA version");
        }
        return info;
    }();
    return result.get();
}

auto get_mode_info(const std::uint16_t mode) {
    auto info = dpmi::typed_dos_memory<mode_info_t>{};
    __dpmi_regs r{};
    r.x.ax = 0x4F01;
    r.x.cx = mode;
    r.x.es = info.segment();
    call(r);
    info.pull();
    return info.get();
}

void set_mode(std::uint16_t mode_id) {
    __dpmi_regs r{};
    r.x.ax = 0x4F02;
    r.x.bx = mode_id | 0x4000;
    call(r);
}

void reset_mode() {
    __dpmi_regs regs = {};
    regs.x.ax = 0x0003;
    __dpmi_int(0x10, &regs);
}

} // namespace internal

oem_info_t get_oem_info() {
    const auto& info = internal::get_controller_info();
    const auto read_string = [](dpmi::farptr_t addr) -> std::string {
        char temp[128u];
        dosmemget(dpmi::to_real_addr(addr), sizeof(temp), temp);
        temp[sizeof(temp) - 1] = 0;
        return temp;
    };
    return {read_string(info.oem_string_ptr),
            read_string(info.oem_vendor_name_ptr),
            read_string(info.oem_product_name_ptr),
            read_string(info.oem_product_rev_ptr)};
}

std::uint32_t get_total_memory_size() {
    return internal::get_controller_info().total_memory * 64u * 1024u;
}

std::vector<mode_info_t> get_modes() {
    const auto& controller_info = internal::get_controller_info();
    std::vector<mode_info_t> modes;
    const auto offset = dpmi::to_real_addr(controller_info.video_mode_ptr);
    _farsetsel(_dos_ds);
    for (auto i = 0u;; i += sizeof(std::uint16_t)) {
        const auto id = _farnspeekw(offset + i);
        if (id == 0xFFFF) {
            break;
        }
        const auto mode_info = internal::get_mode_info(id);
        if ((mode_info.mode_attributes & 0x01) == 0) {
            // list only hardware supported modes
            continue;
        }
        if (!(mode_info.mode_attributes & 0x80) || !mode_info.phys_base_ptr) {
            // select only linear frame buffer modes
            continue;
        }
        if (mode_info.bits_per_pixel < 8) {
            continue;
        }
        modes.push_back({id, mode_info.x_resolution, mode_info.y_resolution,
                         mode_info.bits_per_pixel});
    }
    return modes;
}

class framebuffer::impl {
public:
    explicit impl(const internal::mode_info_t& mode_info)
    : m_mode_info{mode_info},
      m_mapping{m_mode_info.phys_base_ptr, get_total_memory_size()} {}

    [[nodiscard]] auto selector() const { return m_mapping.selector(); }

private:
    internal::mode_info_t m_mode_info;
    dpmi::physical_memory_mapping m_mapping;
};

framebuffer::framebuffer(std::uint16_t mode_id) {
    const auto mode_info = internal::get_mode_info(mode_id);
    internal::set_mode(mode_id);
    m_pimpl = std::make_unique<impl>(std::move(mode_info));
}

framebuffer::~framebuffer() { internal::reset_mode(); }
framebuffer::framebuffer(framebuffer&&) noexcept = default;
framebuffer& framebuffer::operator=(framebuffer&&) noexcept = default;

auto framebuffer::selector() const -> int { return m_pimpl->selector(); }

void framebuffer::write(std::uint32_t offset, const void* data,
                        std::size_t size) const {
    const auto src_offset = reinterpret_cast<const unsigned>(data);
    movedata(_my_ds(), src_offset, selector(), offset, size);
}

void framebuffer::read(std::uint32_t offset, void* data,
                       std::size_t size) const {
    const auto dst_offset = reinterpret_cast<const unsigned>(data);
    movedata(selector(), offset, _my_ds(), dst_offset, size);
}

} // namespace vbe
