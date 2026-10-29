#ifndef RULESTORE_H
#define RULESTORE_H

#include <vector>
#include "rule.h"

class RuleStore
{
    public:
        // Returns 0 for invalid rules or exhausted IDs; leaves the store unchanged.
        std::uint64_t Add(Rule rule);
        bool Remove(std::uint64_t id);
        std::vector<Rule> List() const;

    private:
        std::vector<Rule> _rules;
        std::uint64_t _nextId = 1;
};

#endif
