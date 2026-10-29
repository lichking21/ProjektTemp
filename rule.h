#ifndef RULE_H
#define RULE_H

#include <cstdint>
#include <string>

enum Scope { Global, Local };
enum MatchType { Exact, Wildcard };

struct Rule
{
    std::uint64_t id;
    Scope scope;
    MatchType type;
    std::string pattern;
    std::string folder;
};

#endif 
