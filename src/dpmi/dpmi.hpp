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
#include <memory>
#include <stdexcept>

namespace dpmi {

using error = std::runtime_error;

struct farptr_t {
    std::uint16_t offset;
    std::uint16_t segment;
};

inline std::uint32_t to_real_addr(std::uint32_t segment, std::uint32_t offset) {
    return (segment << 4) + offset;
}

inline std::uint32_t to_real_addr(const farptr_t addr) {
    return to_real_addr(addr.segment, addr.offset);
}

class dos_memory {
public:
    explicit dos_memory(std::uint32_t size_bytes);
    ~dos_memory();

    // Non-copyable, move-enabled
    dos_memory(const dos_memory&) = delete;
    dos_memory& operator=(const dos_memory&) = delete;
    dos_memory(dos_memory&& other) noexcept;
    dos_memory& operator=(dos_memory&& other) noexcept;

    // Data transfer
    void put(const void* src, std::size_t len, std::size_t offset = 0) const;
    void get(void* dst, std::size_t len, std::size_t offset = 0);

    // Accessors
    [[nodiscard]] auto selector() const { return m_selector; }
    [[nodiscard]] auto segment() const { return m_segment; }
    [[nodiscard]] auto size() const { return m_size; }

private:
    int m_selector;
    std::uint16_t m_segment;
    std::uint32_t m_size;
};

template <typename T>
class typed_dos_memory {
    static_assert(std::is_trivial<T>::value &&
                  std::is_standard_layout<T>::value);

public:
    using value_type = T;

    void push() const { m_buffer.put(&m_value, sizeof(m_value)); }

    auto pull() -> const value_type& {
        m_buffer.get(&m_value, sizeof(m_value));
        return get();
    }

    [[nodiscard]] auto get() const -> const value_type& { return m_value; }
    [[nodiscard]] auto get() -> value_type& { return m_value; }
    [[nodiscard]] auto operator->() const -> const value_type* {
        return &get();
    }
    [[nodiscard]] auto operator->() -> value_type* { return &get(); }
    [[nodiscard]] auto segment() const -> std::uint16_t {
        return m_buffer.segment();
    }

private:
    value_type m_value{};
    dos_memory m_buffer{sizeof(value_type)};
};

class physical_memory_mapping {
public:
    physical_memory_mapping(std::uint32_t phys_addr, std::size_t size);
    ~physical_memory_mapping();

    physical_memory_mapping(const physical_memory_mapping&) = delete;
    physical_memory_mapping& operator=(const physical_memory_mapping&) = delete;
    physical_memory_mapping(physical_memory_mapping&&) noexcept;
    physical_memory_mapping& operator=(physical_memory_mapping&&) noexcept;

    [[nodiscard]] auto selector() const -> int;
    [[nodiscard]] auto size() const -> std::size_t;

private:
    class impl;
    std::unique_ptr<impl> m_pimpl;
};

} // namespace dpmi
