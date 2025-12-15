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

#include <cli.hpp>
#include <dpmi.hpp>
#include <log.hpp>
#include <vbe.hpp>
#include <version.h>

#include <stdexcept>

namespace {

using error = std::runtime_error;

constexpr auto bits_per_byte = 8u;

auto find_best_mode() -> vbe::mode_info_t {

    for (const auto& mode : vbe::get_modes()) {
        if (mode.width >= 640 && mode.height >= 480 &&
            mode.bits_per_pixel >= 16) {
            return mode;
        }
    }
    throw error("No suitable VESA mode found.");
}

auto test_video_memory(const std::uint16_t mode_id,
                       const std::uint16_t bus_width,
                       const std::uint8_t num_chips) {

    const auto fb = vbe::framebuffer{mode_id};
    const auto size = vbe::get_total_memory_size();
    const auto bus_width_in_bytes = bus_width / bits_per_byte;
    const auto bytes_per_chip = bus_width_in_bytes / num_chips;

    auto result = std::vector<bool>(num_chips, true);
    auto block = std::vector<std::uint8_t>(bus_width_in_bytes * 1024u, 0u);

    const auto test_pass = [&](const auto& generator) {
        for (auto addr = 0u; addr < size; addr += block.size()) {
            const auto rest = std::min(size - addr, block.size());
            for (auto i = 0u; i < rest; i++) {
                block[i] = generator(i);
            }
            fb.write(addr, block.data(), rest);
        }
        for (auto addr = 0u; addr < size; addr += block.size()) {
            const auto rest = std::min(size - addr, block.size());
            fb.read(addr, block.data(), rest);
            for (auto i = 0u; i < rest; i++) {
                if (block[i] != generator(i)) {
                    result[(i % bus_width_in_bytes) / bytes_per_chip] = false;
                }
            }
        }
    };

    test_pass([](std::uint8_t) { return 0x00; });
    test_pass([](std::uint8_t) { return 0xFF; });
    test_pass([](std::uint8_t) { return 0xAA; });
    test_pass([](std::uint8_t) { return 0x55; });
    test_pass([](std::uint8_t offs) { return offs; });

    return result;
}

void check_arguments(const std::uint16_t bus_width,
                     const std::uint8_t num_chips) {
    if (bus_width < bits_per_byte) {
        throw error("Memory bus width has to be at least 8-bit");
    }
    if (num_chips == 0u) {
        throw error("Invalid number of chips");
    }
    if (bus_width / num_chips < bits_per_byte) {
        throw error("Cards with less than a byte per chip are not supported");
    }
}

void run(const std::uint16_t bus_width, const std::uint8_t num_chips) {

    check_arguments(bus_width, num_chips);

    const auto oem_info = vbe::get_oem_info();
    const auto total_memory = vbe::get_total_memory_size();
    const auto mode = find_best_mode();

    log("Test Info:");
    log("----------");
    log("OEM String: %s", oem_info.description);
    log("Vendor: %s", oem_info.vendor_name);
    log("Product: %s", oem_info.product_name);
    log("Revision: %s", oem_info.revision_name);
    log("Total Memory: %dMB", total_memory / (1024u * 1024u));
    log("Memory bus: %d-bit", bus_width);
    log("Number of chips: %d", num_chips);
    log("Test video mode: %#X [%dx%dx%d]", mode.id, mode.width, mode.height,
        mode.bits_per_pixel);

    log("\nThe test can take up to several minutes");
    log("Press [ENTER] to continue");
    getchar();
    const auto test_result = test_video_memory(mode.id, bus_width, num_chips);
    for (auto i = 0u; i < test_result.size(); i++) {
        log("Chip %d: %s", i, test_result[i] ? "OK" : "BAD");
    }
}

} // namespace

int main(int argc, const char* argv[]) {
    try {
        log("Necroware's Video Memory Tester");
        log("Version " PROJECT_VERSION " (build date " PROJECT_BUILD_DATE ")\n");

        const auto params = std::vector<cli::param_decl>{
            {"chips", true, 0, "Number of chips on the card"},
            {"bus", true, 0, "Memory bus width in bits"},
        };

        const auto args = cli::args_parser{argc, argv, params};
        if (args.wants_help()) {
            args.print_usage();
            return EXIT_SUCCESS;
        }

        run(args.get<int>("bus"), args.get<int>("chips"));
        return EXIT_SUCCESS;
    } catch (std::exception& ex) {
        log("error: %s", ex.what());
        return EXIT_FAILURE;
    }
}
