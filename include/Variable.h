#pragma once
#include <set>
#include <functional>

#include "types.h"
#include "Domain.h"

/**
 * will be passed around via pointers
 */
class Variable
{
public:
    Variable(const Domain& domain, const char *name="");
    int evaluate(SearchNode &context, int time);
    const Domain& getInitialDomain() const;
    const Domain& getDomain(SearchNode &context, int time);

    friend bool operator <(const Variable &a, const Variable &b) {
        return &a < &b;
    }
    friend bool operator ==(const Variable &a, const Variable &b) {
        return &a == &b;
    }
    const char *mName;
private:
    Domain mInitialDomain;
};
