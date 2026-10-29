#ifndef RULE_MATCHER_H
#define RULE_MATCHER_H

#include "rule.h"

// folder is the parent directory; name is one non-empty entry, not a path.
// ASCII case-insensitive only: no Unicode case folding or normalization.
// Folder comparison is lexical, treats / and backslash as separators, and
// ignores trailing separators while preserving slash roots and drive roots.
// Does not resolve dot segments, symlinks, or Windows device-path aliases.
bool Matches(const Rule& rule, const std::string& folder, const std::string& name);

#endif
