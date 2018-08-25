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
    Variable(const domain_t& domain, const char *name="");
    int evaluate(SearchNode &context, int time);
    const domain_t& getInitialDomain() const;
    const domain_t& getDomain(SearchNode &context, int time);

    friend bool operator <(const Variable &a, const Variable &b) {
        return &a < &b;
    }
    friend bool operator ==(const Variable &a, const Variable &b) {
        return &a == &b;
    }
    const char *mName;
private:
    domain_t mInitialDomain;
};
