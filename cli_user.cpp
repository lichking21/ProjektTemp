#include "cli_user.h"
#include "RuleEngine.h"

#include <charconv>
#include <iomanip>
#include <iostream>
#include <string>

namespace {
bool Read(const char* prompt, std::string& value)
{
    std::cout << prompt;
    return static_cast<bool>(std::getline(std::cin, value));
}
}

void run_cli(RuleStore& store)
{
    RuleEngine engine(store);
    std::cout << "Rule Engine (in memory; ASCII case-insensitive matching)\n"
                 "Commands: add, list, remove, check, exit\n";
    std::string command;
    while (Read("> ", command)) {
        if (command == "exit")
            return;
        if (command == "add") {
            Rule rule{};
            std::string scope, type;
            if (!Read("Scope (global/local): ", scope) ||
                !Read("Match (exact/wildcard): ", type) ||
                !Read("Pattern: ", rule.pattern) ||
                !Read("Folder (empty for global): ", rule.folder))
                return;
            if ((scope != "global" && scope != "local") ||
                (type != "exact" && type != "wildcard")) {
                std::cout << "Invalid scope or match type. Rule not added.\n";
                continue;
            }
            rule.scope = scope == "global" ? Global : Local;
            rule.type = type == "exact" ? Exact : Wildcard;
            const std::uint64_t id = store.Add(rule);
            if (id == 0)
                std::cout << "Rule not added: pattern must be non-empty, local folder must be "
                             "non-empty, and global folder must be empty (or IDs exhausted).\n";
            else
                std::cout << "Added rule " << id << '\n';
        } else if (command == "list") {
            const auto rules = store.List();
            if (rules.empty())
                std::cout << "No rules.\n";
            for (const Rule& rule : rules)
                std::cout << "ID=" << rule.id
                          << " scope=" << (rule.scope == Global ? "global" : "local")
                          << " type=" << (rule.type == Exact ? "exact" : "wildcard")
                          << " pattern=" << std::quoted(rule.pattern)
                          << " folder=" << std::quoted(rule.folder) << '\n';
        } else if (command == "remove") {
            std::string input;
            if (!Read("Rule ID: ", input))
                return;
            std::uint64_t id = 0;
            const auto result = std::from_chars(input.data(), input.data() + input.size(), id);
            if (result.ec != std::errc{} || result.ptr != input.data() + input.size() || id == 0)
                std::cout << "Invalid ID: enter a positive 64-bit integer.\n";
            else
                std::cout << (store.Remove(id) ? "Rule removed.\n" : "Rule ID not found.\n");
        } else if (command == "check") {
            std::string folder, name;
            if (!Read("Parent folder: ", folder) || !Read("Entry name: ", name))
                return;
            if (name.empty() || name == "." || name == ".." ||
                name.find_first_of("/\\") != std::string::npos) {
                std::cout << "Invalid name: enter a single non-empty entry name.\n";
                continue;
            }
            std::cout << (engine.ShouldHide(folder, name) ? "Hidden\n" : "Visible\n");
        } else {
            std::cout << "Unknown command. Use add, list, remove, check, or exit.\n";
        }
    }
}
