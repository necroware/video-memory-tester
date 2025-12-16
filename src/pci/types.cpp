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

#include "types.hpp"

namespace pci {

auto to_string(version_t ver) -> std::string {
    char buf[8];
    std::snprintf(buf, sizeof(buf), "%d.%d", ver.major, ver.minor);
    return {buf, sizeof(buf)};
}

auto to_string(address_t addr) -> std::string {
    char buf[8];
    std::snprintf(buf, sizeof(buf), "%02X:%02X.%X", addr.bus, addr.dev,
                  addr.fun);
    return {buf, sizeof(buf)};
}

auto to_string(device_id_t dev) -> std::string {
    char buf[10];
    std::snprintf(buf, sizeof(buf), "%04X:%04X", dev.vendor_id, dev.product_id);
    return {buf, sizeof(buf)};
}

auto to_string(device_class_t id) -> std::string_view {
    static constexpr std::string_view names[] = {
        "Unclassified",            // 0x00
        "Mass Storage Controller", // 0x01
        "Network Controller",      // 0x02
        "Display Controller",      // 0x03
        "Multimedia Controller",   // 0x04
        "Memory Controller",       // 0x05
        "Bridge Device",           // 0x06
        "Simple Communication",    // 0x07
        "Base System Peripheral",  // 0x08
        "Input Device",            // 0x09
        "Docking Station",         // 0x0A
        "Processor",               // 0x0B
        "Serial Bus Controller",   // 0x0C
        "Wireless Controller",     // 0x0D
        "Intelligent Controller",  // 0x0E
        "Satellite Communication", // 0x0F
        "Encryption Controller",   // 0x10
        "Signal Processing",       // 0x11
    };

    const auto idx = static_cast<std::uint8_t>(id);
    return (idx < std::size(names)) ? names[idx] : "Unknown";
}

auto to_string(device_subclass_t id) -> std::string_view {
    using enum device_subclass_t;
    switch (id) {
    case unclassified_vga_incompatible:
        return "Unclassified VGA incompatible";
    case unclassified_vga_compatible:
        return "Unclassified VGA compatible";
    case storage_controller_scsi:
        return "Storage Controller - SCSI";
    case storage_controller_ide:
        return "Storage Controller - IDE";
    case storage_controller_floppy:
        return "Storage Controller - Floppy";
    case storage_controller_ipi:
        return "Storage Controller - IPI";
    case storage_controller_raid:
        return "Storage Controller - RAID";
    case storage_controller_ata:
        return "Storage Controller - ATA";
    case storage_controller_sata:
        return "Storage Controller - SATA";
    case storage_controller_sas:
        return "Storage Controller - SAS";
    case storage_controller_nvm:
        return "Storage Controller - NVM";
    case storage_controller_other:
        return "Storage Controller - Other";
    case network_controller_ethernet:
        return "Network Controller - Ethernet";
    case network_controller_token_ring:
        return "Network Controller - Token Ring";
    case network_controller_fddi:
        return "Network Controller - FDDI";
    case network_controller_atm:
        return "Network Controller - ATM";
    case network_controller_isdn:
        return "Network Controller - ISDN";
    case network_controller_world_fip:
        return "Network Controller - WorldFIP";
    case network_controller_picmg:
        return "Network Controller - PICMG";
    case network_controller_infiniband:
        return "Network Controller - InfiniBand";
    case network_controller_fabric:
        return "Network Controller - Fabric";
    case network_controller_other:
        return "Network Controller - Other";
    case display_controller_vga:
        return "Display Controller - VGA";
    case display_controller_xga:
        return "Display Controller - XGA";
    case display_controller_3d:
        return "Display Controller - 3D";
    case display_controller_other:
        return "Display Controller - Other";
    case multimedia_controller_video:
        return "Multimedia Controller - Video";
    case multimedia_controller_audio:
        return "Multimedia Controller - Audio";
    case multimedia_controller_telephony:
        return "Multimedia Controller - Telephony";
    case multimedia_controller_hd_audio:
        return "Multimedia Controller - HD Audio";
    case multimedia_controller_other:
        return "Multimedia Controller - Other";
    case memory_controller_ram:
        return "Memory Controller - RAM";
    case memory_controller_flash:
        return "Memory Controller - Flash";
    case memory_controller_other:
        return "Memory Controller - Other";
    case bridge_device_host:
        return "Bridge Device - Host";
    case bridge_device_isa:
        return "Bridge Device - ISA";
    case bridge_device_eisa:
        return "Bridge Device - EISA";
    case bridge_device_mca:
        return "Bridge Device - MCA";
    case bridge_device_pci_to_pci:
        return "Bridge Device - PCI-to-PCI";
    case bridge_device_pcmcia:
        return "Bridge Device - PCMCIA";
    case bridge_device_nu_bus:
        return "Bridge Device - NuBus";
    case bridge_device_cardbus:
        return "Bridge Device - CardBus";
    case bridge_device_rsrc_bus:
        return "Bridge Device - Resource Bus";
    case bridge_device_other:
        return "Bridge Device - Other";
    case communication_controller_serial:
        return "Communication Controller - Serial";
    case communication_controller_parallel:
        return "Communication Controller - Parallel";
    case communication_controller_multiport_serial:
        return "Communication Controller - Multiport Serial";
    case communication_controller_modem:
        return "Communication Controller - Modem";
    case communication_controller_ieee488:
        return "Communication Controller - IEEE488";
    case communication_controller_other:
        return "Communication Controller - Other";
    case system_controller_interrupt:
        return "System Peripheral - Interrupt Controller";
    case system_controller_dma:
        return "System Peripheral - DMA Controller";
    case system_controller_timer:
        return "System Peripheral - Timer";
    case system_controller_rtc:
        return "System Peripheral - RTC";
    case system_controller_hotplug:
        return "System Peripheral - Hotplug Controller";
    case system_controller_other:
        return "System Peripheral - Other";
    case input_device_keyboard:
        return "Input Device - Keyboard";
    case input_device_digitizer:
        return "Input Device - Digitizer";
    case input_device_mouse:
        return "Input Device - Mouse";
    case input_device_scanner:
        return "Input Device - Scanner";
    case input_device_gameport:
        return "Input Device - Gameport";
    case input_device_other:
        return "Input Device - Other";
    case docking_station_generic:
        return "Docking Station - Generic";
    case docking_station_other:
        return "Docking Station - Other";
    case processor_386:
        return "Processor - 386";
    case processor_486:
        return "Processor - 486";
    case processor_pentium:
        return "Processor - Pentium";
    case processor_alpha:
        return "Processor - Alpha";
    case processor_powerpc:
        return "Processor - PowerPC";
    case processor_mips:
        return "Processor - MIPS";
    case processor_co_processor:
        return "Processor - Co-Processor";
    case serial_bus_controller_firewire:
        return "Serial Bus Controller - FireWire";
    case serial_bus_controller_access_bus:
        return "Serial Bus Controller - Access Bus";
    case serial_bus_controller_ssa:
        return "Serial Bus Controller - SSA";
    case serial_bus_controller_usb:
        return "Serial Bus Controller - USB";
    case serial_bus_controller_fibre_channel:
        return "Serial Bus Controller - Fibre Channel";
    case serial_bus_controller_smbus:
        return "Serial Bus Controller - SMBus";
    case serial_bus_controller_infiniband_bus:
        return "Serial Bus Controller - InfiniBand Bus";
    case serial_bus_controller_ipmi:
        return "Serial Bus Controller - IPMI";
    case serial_bus_controller_other:
        return "Serial Bus Controller - Other";
    case wireless_controller_irda:
        return "Wireless Controller - IRDA";
    case wireless_controller_consumer_ir:
        return "Wireless Controller - Consumer IR";
    case wireless_controller_rf_controller:
        return "Wireless Controller - RF";
    case wireless_controller_bluetooth:
        return "Wireless Controller - Bluetooth";
    case wireless_controller_broadband:
        return "Wireless Controller - Broadband";
    case wireless_controller_ethernet_80211:
        return "Wireless Controller - Ethernet (802.11)";
    case wireless_controller_other:
        return "Wireless Controller - Other";
    case inteligent_controller_i2o:
        return "Intelligent I/O Controller";
    case satellite_communication_tv:
        return "Satellite Communication - TV";
    case satellite_communication_audio:
        return "Satellite Communication - Audio";
    case satellite_communication_voice:
        return "Satellite Communication - Voice";
    case satellite_communication_data:
        return "Satellite Communication - Data";
    case encryption_controller_network:
        return "Encryption/Decryption - Network";
    case encryption_controller_entertainment:
        return "Encryption/Decryption - Entertainment";
    case encryption_controller_other:
        return "Encryption/Decryption - Other";
    case signal_processing_dpio:
        return "Data Acquisition - DPIO";
    case signal_processing_other:
        return "Data Acquisition - Other";
    }

    // Fallback and print at least the class name
    const auto class_id = static_cast<std::uint16_t>(id) >> 16;
    return to_string(static_cast<device_class_t>(class_id));
}

} // namespace pci
