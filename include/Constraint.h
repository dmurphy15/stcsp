#pragma once

#include <set>
#include <functional>

#include "types.h"



/*
 * Interface that all constraints should satisfy.
 *
 * normalize takes a list for constraints an new variables that would be equivalent to
 *  the current constraint. With the resulting lists from a call to normalize, a solver
 *  should be able to delete the constraint and add the new constraints and variables
 *
 * isSatisfied takes a time and returns 1 if the constraint is satisfied then, and 0 otherwise.
 *
 */

class Constraint
{
public:
    Constraint(std::initializer_list<Expression_r> expressions, bool symmetric);

    // again a default implementation is provided; should be overridden if you're making a constraint
    // with special normalization rules
    // if you're not going to use this, then you don't have to implement build if you dont want to
    virtual void normalize(std::set<Constraint_r> &constraintList,
                           std::set<Variable_r> &variableList);

    // used for solving an instantaneous csp
    virtual bool isSatisfied(SearchNode &context, int time) const = 0;

    // used by instantaneous csp to set up mappings from vars to constraints and vice versa,
    // which it can later use for GAC, etc
    void getVariables(std::set<Variable_r>& variables) const;

    ////remember that whenever you return something from propagate, it MUST have length prefixK

    // change the domain of the specified variable in the given context to comply with this constraint
    // and the domains of the other variables involved
    // could just call the context's default propagator
    // return whatever values it has removed from the variable's domain
            // propagates over all timepoints in the context
    virtual std::vector<std::set<int>> propagate(Variable &v, SearchNode &context) = 0;

    friend bool operator< (const Constraint &lhs, const Constraint &rhs);
    friend bool operator== (const Constraint &lhs, const Constraint &rhs);
private:
    std::vector<Expression_r> mExpressions;
    virtual Constraint& build(std::vector<Expression_r>& expressions);
};