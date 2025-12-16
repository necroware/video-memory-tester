#include "io.hpp"

#include <dpmi.h>
#include <pc.h>

namespace pci {

namespace {

constexpr auto data_port = 0xCFC;

auto cfg_call(const address_t& addr, const std::uint16_t reg) {
    const auto enable = static_cast<std::uint32_t>(1u) << 31;
    const auto bus_bits = static_cast<std::uint32_t>(addr.bus) << 16;
    const auto dev_bits = static_cast<std::uint32_t>(addr.dev) << 11;
    const auto fun_bits = static_cast<std::uint32_t>(addr.fun) << 8;
    const auto reg_bits = reg & 0xFC;
    outportl(0xCF8, enable | bus_bits | dev_bits | fun_bits | reg_bits);
}

} // namespace

auto read_dword(const address_t& addr, const std::uint16_t reg)
    -> std::uint32_t {
    cfg_call(addr, reg);
    return inportl(data_port);
}

auto read_word(const address_t& addr, const std::uint16_t reg)
    -> std::uint16_t {
    cfg_call(addr, reg);
    return inportw(data_port + (reg & 2u));
}

auto read_byte(const address_t& addr, const std::uint16_t reg) -> std::uint8_t {
    cfg_call(addr, reg);
    return inportb(data_port + (reg & 3u));
}

void write_dword(const address_t& addr, const std::uint16_t reg,
                 const std::uint32_t value) {
    cfg_call(addr, reg);
    outportl(data_port, value);
}

auto write_word(const address_t& addr, const std::uint16_t reg,
                const std::uint16_t value) -> void {
    cfg_call(addr, reg);
    outportw(data_port + (reg & 2u), value);
}

auto write_byte(const address_t& addr, const std::uint16_t reg,
                const std::uint8_t value) -> void {
    cfg_call(addr, reg);
    outportb(data_port + (reg & 3u), value);
}

} // namespace pci
