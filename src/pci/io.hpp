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

#include "types.hpp"

namespace pci {

auto read_byte(const address_t& addr, const std::uint16_t reg) -> std::uint8_t;
auto read_word(const address_t& addr, const std::uint16_t reg) -> std::uint16_t;
auto read_dword(const address_t& addr, const std::uint16_t reg) -> std::uint32_t;

void write_byte(const address_t& addr, const std::uint16_t reg, std::uint8_t val);
void write_word(const address_t& addr, const std::uint16_t reg, std::uint16_t val);
void write_dword(const address_t& addr, const std::uint16_t reg, std::uint32_t val);

} // namespace pci
