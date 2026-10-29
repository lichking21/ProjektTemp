#ifndef RULE_ENGINE_H
#define RULE_ENGINE_H

#include "rule_store.h"

class RuleEngine
{
public:
    // The store must outlive this engine; subsequent changes are visible.
    explicit RuleEngine(const RuleStore& store) : _store(store) {}
    bool ShouldHide(const std::string& folder, const std::string& name) const;

private:
    const RuleStore& _store;
};

#endif
