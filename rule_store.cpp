#include "rule_store.h"
#include <algorithm>

std::uint64_t RuleStore::Add(Rule rule)
{
    if (rule.pattern.empty() ||
        (rule.scope != Global && rule.scope != Local) ||
        (rule.type != Exact && rule.type != Wildcard) ||
        (rule.scope == Local && rule.folder.empty()) ||
        (rule.scope == Global && !rule.folder.empty()) || _nextId == 0)
        return 0;

    rule.id = _nextId;

    _rules.push_back(rule);

    return _nextId++;
}

bool RuleStore::Remove(std::uint64_t id)
{
    auto rule = std::find_if(_rules.begin(), _rules.end(),
        [id](const Rule& r){
            return r.id == id;
        });

    if (rule == _rules.end())
        return false;

    _rules.erase(rule);

    return true;
}

std::vector<Rule> RuleStore::List() const
{
    return _rules;
}
