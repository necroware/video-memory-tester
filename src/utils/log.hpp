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

#include <cstdio>
#include <string>

namespace log_internal {

template <typename T>
auto wrap_arg(T&& arg) {
    return std::forward<T>(arg);
}

inline const char* wrap_arg(const std::string& s) { return s.c_str(); }

} // namespace log_internal

template <typename... Args>
void log(const char* fmt, Args&&... args) {
    std::printf(fmt, log_internal::wrap_arg(std::forward<Args>(args))...);
    std::puts("");
}

