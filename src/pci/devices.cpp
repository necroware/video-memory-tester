#include "devices.hpp"

#include "io.hpp"

namespace pci {

namespace {

constexpr auto max_bus = std::uint8_t{255u};
constexpr auto max_dev = std::uint8_t{31u};
constexpr auto max_fun = std::uint8_t{7u};
constexpr auto invalid = address_t{max_bus, max_dev, max_fun + 1};

auto clamp(address_t addr) -> address_t {
    // Limit out-of-range values. Bus is limited by
    // std::uint8_t, so we don't need to check that.
    if (addr.dev > max_dev || addr.fun > max_fun) {
        return invalid;
    }
    return addr;
}

auto next(address_t addr) -> address_t {
    if (addr.fun > max_fun || addr.fun++ < max_fun) {
        return addr;
    }
    addr.fun = 0;
    if (addr.dev++ < max_dev) {
        return addr;
    }
    addr.dev = 0;
    if (addr.bus++ < max_bus) {
        return addr;
    }
    // reached the end
    return invalid;
}

auto find_next_device(address_t addr) -> std::optional<device_t> {
    while (addr != invalid) {
        auto device = device_t::access(addr);
        if (device) {
            return device;
        }
        addr.fun = max_fun;
        addr = next(addr);
    }
    return std::nullopt;
}

} // namespace

auto device_t::access(address_t addr) -> std::optional<device_t> {
    const auto value = read_dword(addr, 0x00);
    if (value == 0xFFFFFFFF) {
        return std::nullopt;
    }
    if (addr.fun > 0) {
        const auto base = address_t{addr.bus, addr.dev, 0u};
        // Is multi device?
        if ((read_byte(base, 0x0E) & 0x80) == 0) {
            return std::nullopt;
        }
    }
    const auto vendor_id = static_cast<std::uint16_t>(value & 0xFFFF);
    const auto product_id = static_cast<std::uint16_t>(value >> 16);
    return device_t{addr, {vendor_id, product_id}};
}

auto device_t::get_class() const -> device_class_t {
    return static_cast<device_class_t>(read_byte(m_addr, 0x0B));
}

auto device_t::get_subclass() const -> device_subclass_t {
    return static_cast<device_subclass_t>(read_word(m_addr, 0x0A));
}

device_iterator_t::device_iterator_t(address_t addr)
: m_device{find_next_device(clamp(addr))} {}

void device_iterator_t::advance() {
    if (m_device) {
        const auto addr = m_device->get_address();
        m_device = find_next_device(next(addr));
    }
}

} // namespace pci
