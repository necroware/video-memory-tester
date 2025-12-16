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

#pragma once

#include <compare>
#include <cstdint>
#include <string>

namespace pci {

struct version_t {
    std::uint8_t major;
    std::uint8_t minor;
    auto operator<=>(const version_t&) const = default;
};

auto to_string(version_t) -> std::string;

struct address_t {
    std::uint8_t bus;
    std::uint8_t dev;
    std::uint8_t fun;
    auto operator<=>(const address_t&) const = default;
};

auto to_string(address_t) -> std::string;

struct device_id_t {
    std::uint16_t vendor_id;
    std::uint16_t product_id;
    auto operator<=>(const device_id_t&) const = default;
};

auto to_string(device_id_t) -> std::string;

enum class device_class_t : std::uint8_t {
    unclassified = 0x00,
    storage_controller = 0x01,
    network_controller = 0x02,
    display_controller = 0x03,
    multimedia_controller = 0x04,
    memory_controller = 0x05,
    bridge_device = 0x06,
    simple_communication = 0x07,
    base_system_peripheral = 0x08,
    input_device = 0x09,
    docking_station = 0x0A,
    processor = 0x0B,
    serial_bus_controller = 0x0C,
    wireless_controller = 0x0D,
    intelligent_controller = 0x0E,
    satellite_communication = 0x0F,
    encryption_controller = 0x10,
    signal_processing = 0x11,
};

auto to_string(device_class_t code) -> std::string_view;

enum class device_subclass_t : std::uint16_t {

    // 0x00: Pre-PCI devices
    unclassified_vga_incompatible = 0x0000,
    unclassified_vga_compatible = 0x0001,

    // 0x01: Mass Storage Controller
    storage_controller_scsi = 0x0100,
    storage_controller_ide = 0x0101,
    storage_controller_floppy = 0x0102,
    storage_controller_ipi = 0x0103,
    storage_controller_raid = 0x0104,
    storage_controller_ata = 0x0105,
    storage_controller_sata = 0x0106,
    storage_controller_sas = 0x0107,
    storage_controller_nvm = 0x0108,
    storage_controller_other = 0x0180,

    // 0x02: Network Controller
    network_controller_ethernet = 0x0200,
    network_controller_token_ring = 0x0201,
    network_controller_fddi = 0x0202,
    network_controller_atm = 0x0203,
    network_controller_isdn = 0x0204,
    network_controller_world_fip = 0x0205,
    network_controller_picmg = 0x0206,
    network_controller_infiniband = 0x0207,
    network_controller_fabric = 0x0208,
    network_controller_other = 0x0280,

    // 0x03: Display Controller
    display_controller_vga = 0x0300,
    display_controller_xga = 0x0301,
    display_controller_3d = 0x0302,
    display_controller_other = 0x0380,

    // 0x04: Multimedia Controller
    multimedia_controller_video = 0x0400,
    multimedia_controller_audio = 0x0401,
    multimedia_controller_telephony = 0x0402,
    multimedia_controller_hd_audio = 0x0403,
    multimedia_controller_other = 0x0480,

    // 0x05: Memory Controller
    memory_controller_ram = 0x0500,
    memory_controller_flash = 0x0501,
    memory_controller_other = 0x0580,

    // 0x06: Bridge Device
    bridge_device_host = 0x0600,
    bridge_device_isa = 0x0601,
    bridge_device_eisa = 0x0602,
    bridge_device_mca = 0x0603,
    bridge_device_pci_to_pci = 0x0604,
    bridge_device_pcmcia = 0x0605,
    bridge_device_nu_bus = 0x0606,
    bridge_device_cardbus = 0x0607,
    bridge_device_rsrc_bus = 0x0608,
    bridge_device_other = 0x0680,

    // 0x07: Communication Controller
    communication_controller_serial = 0x0700,
    communication_controller_parallel = 0x0701,
    communication_controller_multiport_serial = 0x0702,
    communication_controller_modem = 0x0703,
    communication_controller_ieee488 = 0x0704,
    communication_controller_other = 0x0780,

    // 0x08: System Peripheral
    system_controller_interrupt = 0x0800,
    system_controller_dma = 0x0801,
    system_controller_timer = 0x0802,
    system_controller_rtc = 0x0803,
    system_controller_hotplug = 0x0804,
    system_controller_other = 0x0880,

    // 0x09: Input Device
    input_device_keyboard = 0x0900,
    input_device_digitizer = 0x0901,
    input_device_mouse = 0x0902,
    input_device_scanner = 0x0903,
    input_device_gameport = 0x0904,
    input_device_other = 0x0980,

    // 0x0A: Docking Station
    docking_station_generic = 0x0A00,
    docking_station_other = 0x0A80,

    // 0x0B: Processor
    processor_386 = 0x0B00,
    processor_486 = 0x0B01,
    processor_pentium = 0x0B02,
    processor_alpha = 0x0B10,
    processor_powerpc = 0x0B20,
    processor_mips = 0x0B30,
    processor_co_processor = 0x0B40,

    // 0x0C: Serial Bus Controller
    serial_bus_controller_firewire = 0x0C00,
    serial_bus_controller_access_bus = 0x0C01,
    serial_bus_controller_ssa = 0x0C02,
    serial_bus_controller_usb = 0x0C03,
    serial_bus_controller_fibre_channel = 0x0C04,
    serial_bus_controller_smbus = 0x0C05,
    serial_bus_controller_infiniband_bus = 0x0C06,
    serial_bus_controller_ipmi = 0x0C07,
    serial_bus_controller_other = 0x0C80,

    // 0x0D: Wireless Controller
    wireless_controller_irda = 0x0D00,
    wireless_controller_consumer_ir = 0x0D01,
    wireless_controller_rf_controller = 0x0D10,
    wireless_controller_bluetooth = 0x0D11,
    wireless_controller_broadband = 0x0D12,
    wireless_controller_ethernet_80211 = 0x0D20,
    wireless_controller_other = 0x0D80,

    // 0x0E: Intelligent I/O
    inteligent_controller_i2o = 0x0E00,

    // 0x0F: Satellite Communication
    satellite_communication_tv = 0x0F00,
    satellite_communication_audio = 0x0F01,
    satellite_communication_voice = 0x0F02,
    satellite_communication_data = 0x0F03,

    // 0x10: Encryption/Decryption
    encryption_controller_network = 0x1000,
    encryption_controller_entertainment = 0x1001,
    encryption_controller_other = 0x1080,

    // 0x11: Data Acquisition
    signal_processing_dpio = 0x1100,
    signal_processing_other = 0x1180
};

auto to_string(device_subclass_t id) -> std::string_view;

} // namespace pci
