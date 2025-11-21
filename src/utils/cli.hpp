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

#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace cli {

using error = std::runtime_error;

using value_variant = std::variant<bool, int, std::string, double>;

struct param_decl {
    std::string name;
    bool required;
    value_variant default_value;
    std::string description;
};

class args_parser {
public:
    args_parser(int argc, const char* argv[], std::vector<param_decl> params)
    : m_program_name{argv[0]}, m_decls{std::move(params)} {

        // Always add default options
        m_decls.push_back({"help", false, false, "Show usage information"});

        // Fill default values
        for (const auto& decl : m_decls) {
            if (!decl.required) {
                m_values[decl.name] = decl.default_value;
            }
        }

        // parse arguments
        for (int i = 1; i < argc; ++i) {
            parse_argument(argv[i]);
        }

        if (wants_help()) {
            return;
        }

        // check required
        for (const auto& decl : m_decls) {
            if (decl.required && !m_values.contains(decl.name)) {
                throw error("missing required parameter: --" + decl.name);
            }
        }
    }

    [[nodiscard]] auto has(const std::string& key) const -> bool {
        return m_values.contains(key);
    }

    template <typename T>
    [[nodiscard]] auto get(const std::string& key) const -> T {
        try {
            return std::get<T>(m_values.at(key));
        } catch (...) {
            throw error("invalid value type for --" + key);
        }
    }

    [[nodiscard]] auto wants_help() const -> bool {
        return std::get<bool>(m_values.at("help"));
    }

    void print_usage() const {
        auto msg = std::string{"usage: "} + m_program_name;
        msg += " [options] [args...]\noptions:\n";
        for (const auto& decl : m_decls) {
            msg += "  --" + decl.name;
            std::visit(dispatch{
                           [&msg](bool) {},
                           [&msg](int) { msg += "=<int>"; },
                           [&msg](double) { msg += "=<float>"; },
                           [&msg](const std::string&) { msg += "=<string>"; },
                       },
                       decl.default_value);
            msg += "\n    " + decl.description;
            if (!decl.required) {
                msg += " (default: ";
                std::visit(
                    dispatch{
                        [&](const std::string& val) { msg += val; },
                        [&](bool val) { msg += val ? "true" : "false"; }, 
                        [&](const auto& val) { msg += std::to_string(val); },
                    },
                    decl.default_value);
                msg += ")\n";
            } else {
                msg += " [required]\n";
            }
        }
        std::fputs(msg.c_str(), stderr);
    }

private:
    template <class... Ts>
    struct dispatch : Ts... {
        using Ts::operator()...;
    };

    static auto from_string(const param_decl& decl, const std::string& str) {
        try {
            return std::visit(
                dispatch{
                    [&](int) -> value_variant { return std::stoi(str); },
                    [&](double) -> value_variant { return std::stod(str); },
                    [&](const std::string&) -> value_variant { return str; },
                },
                decl.default_value);
        } catch (...) {
            throw error("invalid value for --" + decl.name + ": " + str);
        }
    }

    std::string m_program_name;
    std::unordered_map<std::string, value_variant> m_values;
    std::vector<param_decl> m_decls;

    auto find_decl(const std::string& key) const -> const param_decl& {
        for (const auto& decl : m_decls) {
            if (decl.name == key) {
                return decl;
            }
        }
        throw error("unknown parameter: --" + key);
    }

    void parse_argument(const std::string& arg) {
        if (!arg.starts_with("--")) {
            throw error("unknown parameter: " + arg);
        }
        const auto eq = arg.find('=');
        const auto key = arg.substr(2, eq == std::string::npos ? eq : eq - 2);
        const auto& decl = find_decl(key);
        if (std::holds_alternative<bool>(decl.default_value)) {
            m_values[key] = true;
        } else if (eq != std::string::npos) {
            const auto value = arg.substr(eq + 1);
            m_values[key] = from_string(decl, value);
        } else {
            throw error("missing value for --" + key);
        }
    }
};

} // namespace cli

