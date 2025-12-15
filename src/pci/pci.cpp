// Necrowares's Video Memory Tester
// Copyright (C) 2025 by Necroware
//
// This program is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pci.hpp"

#include <dpmi.h>

namespace pci {

auto get_version() -> std::optional<version_t> {
    auto r = __dpmi_regs{};
    r.x.ax = 0xB101;
    __dpmi_int(0x1A, &r);
    if (r.x.flags & 1) {
        return std::nullopt;
    }
    return version_t{r.h.bh, r.h.bl};
}

} // namespace pci

