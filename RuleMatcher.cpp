#include "RuleMatcher.h"

#include <string_view>

namespace {
char FoldAscii(char value)
{
    return value >= 'A' && value <= 'Z' ? static_cast<char>(value + ('a' - 'A')) : value;
}

std::string NormalizeFolder(std::string folder)
{
    for (char& value : folder)
        value = value == '/' ? '\\' : FoldAscii(value);

    const bool driveRoot = folder.size() >= 3 &&
        folder[0] >= 'a' && folder[0] <= 'z' && folder[1] == ':' && folder[2] == '\\';
    const std::size_t minimum = driveRoot ? 3 : 1;
    while (folder.size() > minimum && folder.back() == '\\')
        folder.pop_back();
    return folder;
}

bool WildcardMatches(std::string_view pattern, std::string_view name)
{
    std::size_t p = 0, n = 0;
    std::size_t star = std::string_view::npos, retry = 0;
    while (n < name.size()) {
        if (p < pattern.size() && pattern[p] == '*') {
            star = p++;
            retry = n;
        } else if (p < pattern.size() &&
                   (pattern[p] == '?' || FoldAscii(pattern[p]) == FoldAscii(name[n]))) {
            ++p;
            ++n;
        } else if (star != std::string_view::npos) {
            p = star + 1;
            n = ++retry;
        } else {
            return false;
        }
    }
    while (p < pattern.size() && pattern[p] == '*')
        ++p;
    return p == pattern.size();
}
}

bool Matches(const Rule& rule, const std::string& folder, const std::string& name)
{
    if (rule.pattern.empty() || name.empty() || name == "." || name == ".." ||
        name.find_first_of("/\\") != std::string::npos)
        return false;

    if (rule.scope == Local) {
        if (rule.folder.empty() || NormalizeFolder(rule.folder) != NormalizeFolder(folder))
            return false;
    } else if (rule.scope != Global || !rule.folder.empty()) {
        return false;
    }

    if (rule.type == Wildcard)
        return WildcardMatches(rule.pattern, name);
    if (rule.type != Exact || rule.pattern.size() != name.size())
        return false;
    for (std::size_t i = 0; i < name.size(); ++i)
        if (FoldAscii(rule.pattern[i]) != FoldAscii(name[i]))
            return false;
    return true;
}
