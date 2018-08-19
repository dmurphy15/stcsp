#pragma once
#include <set>
#include <functional>

#include "types.h"

/**
 * will be passed around via pointers
 */
class Variable
{
public:
    Variable(std::set<int> domain);
    int evaluate(SearchNode &context, int time);
    std::set<int> getInitialDomain() const;
    std::set<int> getDomain(SearchNode &context, int time);

    friend bool operator <(const Variable &a, const Variable &b) {
        return &a < &b;
    }
    friend bool operator ==(const Variable &a, const Variable &b) {
        return &a == &b;
    }
private:
    std::set<int> mInitialDomain;
};
