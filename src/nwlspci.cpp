#include <devices.hpp>
#include <pci.hpp>

int main(int argc, const char** argv) {
    const auto version = pci::get_version();
    if (!version) {
        std::printf("PCI BIOS not found!");
        return -1;
    }
    std::printf("Detected PCI version: %s\n", pci::to_string(*version).c_str());
    std::printf("Address | DeviceIDs | Description\n");
    std::printf("--------|-----------|-----------------------------------\n");
    for (const auto& device : pci::devices{}) {
        const auto addr = device.get_address();
        const auto dev_id = device.get_id();
        const auto subclass = device.get_subclass();
        std::printf("%s | %s | %s\n", pci::to_string(addr).c_str(),
                    pci::to_string(dev_id).c_str(),
                    pci::to_string(subclass).data());
    }
    return 0;
}
