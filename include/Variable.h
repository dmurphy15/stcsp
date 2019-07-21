#pragma once
#include <set>
#include <functional>

#include "types.h"

/***************************** Variable ********************************************************************************
 * Represents a variable for the solver. Pretty much just contains a domain and gives us unique objects with which to
 * associate values
 * ********************************************************************************************************************/

class Variable
{
public:
    /**
     * Create a new variable
     * @param domain - the full, initial domain of values the variable can take
     * @param name - a name for the variable
     */
    Variable(const domain_t& domain, const char *name="");
    /**
     * Get the variable's value at a certain timepoint within a certain searchnode
     * @param context - the searchnode we're looking at
     * @param time - the timepoint within that searchnode
     * @return the variable's assigned value
     */
    int evaluate(SearchNode &context, int time);
    /** get the initial domain that the variable started with */
    const domain_t& getInitialDomain() const;
    /**
     * get the domain of the variable within a certain searchnode at a certain timepoint
     * @param context - the searchnode we're looking at
     * @param time - the timepoint within that searchnode
     * @return the variable's domain
     */
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
