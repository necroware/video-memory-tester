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

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

namespace vbe {

struct oem_info_t {
    std::string description;
    std::string vendor_name;
    std::string product_name;
    std::string revision_name;
};

struct mode_info_t {
    std::uint16_t id;
    std::uint16_t width;
    std::uint16_t height;
    std::uint8_t bits_per_pixel;
};

class framebuffer {
public:
    explicit framebuffer(std::uint16_t mode_id);
    ~framebuffer();

    framebuffer(const framebuffer&) = delete;
    framebuffer(framebuffer&&) noexcept;
    framebuffer& operator=(const framebuffer&) = delete;
    framebuffer& operator=(framebuffer&&) noexcept;

    [[nodiscard]] auto selector() const -> int; 
    void write(std::uint32_t offset, const void* data, std::size_t size) const;
    void read(std::uint32_t offset, void* data, std::size_t size) const;

private:
    class impl;
    std::unique_ptr<impl> m_pimpl;
};

oem_info_t get_oem_info();
std::size_t get_total_memory_size();
std::vector<mode_info_t> get_modes();
void reset();

} // namespace
