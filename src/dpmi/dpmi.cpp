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

#include "dpmi.hpp"

#include <dpmi.h>
#include <go32.h>

namespace dpmi {

namespace details {} // namespace details

dos_memory::dos_memory(std::uint32_t size)
: m_selector(-1), m_segment(0), m_size(size) {
    const auto paragraphs = static_cast<int>((size + 15) / 16);
    const auto segment = __dpmi_allocate_dos_memory(paragraphs, &m_selector);
    if (segment == -1 || m_selector == -1) {
        throw error("failed to allocate dos memory");
    }
    m_segment = static_cast<std::uint16_t>(segment);
}

dos_memory::~dos_memory() {
    if (m_selector != -1) {
        __dpmi_free_dos_memory(m_selector);
    }
}

dos_memory::dos_memory(dos_memory&& other) noexcept
: m_selector(other.m_selector),
  m_segment(other.m_segment),
  m_size(other.m_size) {
    other.m_selector = -1;
    other.m_segment = 0;
    other.m_size = 0;
}

dos_memory& dos_memory::operator=(dos_memory&& other) noexcept {
    if (this != &other) {
        auto temp = dos_memory{std::forward<dos_memory>(other)};
        std::swap(m_selector, temp.m_selector);
        std::swap(m_segment, temp.m_segment);
        std::swap(m_size, temp.m_size);
    }
    return *this;
}

void dos_memory::put(const void* src, std::size_t len, std::size_t offset) const {
    dosmemput(src, len, to_real_addr(m_segment, offset));
}

void dos_memory::get(void* dst, std::size_t len, std::size_t offset) {
    dosmemget(to_real_addr(m_segment, offset), len, dst);
}

class physical_memory_mapping::impl {
public:
    impl(std::uint32_t phys_addr, std::uint32_t size) {
        m_meminfo.address = phys_addr;
        m_meminfo.size = size;
        if (__dpmi_physical_address_mapping(&m_meminfo) != 0) {
            throw error("failed to map physical address");
        }
        m_selector = __dpmi_allocate_ldt_descriptors(1);
        if (m_selector < 0) {
            __dpmi_free_physical_address_mapping(&m_meminfo);
            throw error("Failed to allocate LDT descriptor");
        }
        __dpmi_set_segment_base_address(m_selector, m_meminfo.address);
        __dpmi_set_segment_limit(m_selector, m_meminfo.size - 1);
    }

    ~impl() {
        __dpmi_free_ldt_descriptor(m_selector);
        __dpmi_free_physical_address_mapping(&m_meminfo);
    }

    impl(const impl&) = delete;
    impl& operator=(const impl&) = delete;
    impl(impl&&) = delete;
    impl& operator=(impl&&) = delete;

    [[nodiscard]] auto selector() const -> int { return m_selector; }
    [[nodiscard]] auto size() const -> std::uint32_t { return m_meminfo.size; }

private:
    __dpmi_meminfo m_meminfo{};
    int m_selector{-1};
};

physical_memory_mapping::physical_memory_mapping(std::uint32_t phys_addr,
                                                 std::size_t size)
: m_pimpl(std::make_unique<impl>(phys_addr, size)) {}

physical_memory_mapping::~physical_memory_mapping() = default;
physical_memory_mapping::physical_memory_mapping(
    physical_memory_mapping&& other) noexcept = default;
physical_memory_mapping& physical_memory_mapping::operator=(
    physical_memory_mapping&& other) noexcept = default;

auto physical_memory_mapping::selector() const -> int {
    return m_pimpl->selector();
}

auto physical_memory_mapping::size() const -> std::size_t {
    return m_pimpl->size();
}

} // namespace dpmi
