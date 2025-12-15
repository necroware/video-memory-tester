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

#include <iterator>
#include <optional>

namespace pci {

class device_t {
public:
    static auto access(address_t addr) -> std::optional<device_t>;

    auto get_address() const -> address_t { return m_addr; }
    auto get_id() const -> device_id_t { return m_id; }
    auto get_class() const -> device_class_t;
    auto get_subclass() const -> device_subclass_t;

private:
    address_t m_addr;
    device_id_t m_id;

    explicit device_t(address_t addr, device_id_t id)
    : m_addr{addr}, m_id{id} {}
};

class device_iterator_t {
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = device_t;
    using difference_type = std::ptrdiff_t;
    using pointer = const device_t*;
    using reference = const device_t&;

    device_iterator_t() {}
    explicit device_iterator_t(address_t addr);

    auto operator*() const -> reference { return m_device.value(); }
    auto operator->() const -> pointer { return &m_device.value(); }

    auto operator++() -> device_iterator_t& {
        advance();
        return *this;
    }

    auto operator++(int) -> device_iterator_t {
        auto tmp = *this;
        advance();
        return tmp;
    }

    auto operator==(const device_iterator_t& other) const -> bool {
        if (m_device && other.m_device) {
            return m_device->get_address() == other.m_device->get_address();
        }
        return !m_device && !other.m_device;
    }

private:
    std::optional<device_t> m_device;
    void advance();
};

class devices {
public:
    static auto begin() { return device_iterator_t{{0, 0, 0}}; }
    static auto end() { return device_iterator_t{}; }

    static auto find(const device_id_t& id) -> std::optional<device_t> {
        for (const auto& device : devices{}) {
            if (device.get_id() == id) {
                return device;
            }
        }
        return std::nullopt;
    }

    static auto find(const device_class_t id) -> std::optional<device_t> {
        for (const auto& device : devices{}) {
            if (device.get_class() == id) {
                return device;
            }
        }
        return std::nullopt;
    }
};

} // namespace pci

