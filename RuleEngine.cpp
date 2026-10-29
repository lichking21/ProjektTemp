#include "RuleEngine.h"
#include "RuleMatcher.h"

bool RuleEngine::ShouldHide(const std::string& folder, const std::string& name) const
{
    for (const Rule& rule : _store.List())
        if (Matches(rule, folder, name))
            return true;
    return false;
}
