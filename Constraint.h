#pragma once

#include <set>
#include <functional>

class Variable;
using Variable_r = std::reference_wrapper<Variable>;
class Expression;
using Expression_r = std::reference_wrapper<Expression>;
class Constraint;
using Constraint_r = std::reference_wrapper<Constraint>;
class InstantSolver;



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
    /* the consts here mean that the pointer cannot be made to point somewhere else;
     * it essentially forces you to allocate space for the set ahead of time */
    virtual void normalize(std::set<Constraint_r> &constraintList,
                           std::set<Variable_r> &variableList) = 0;

    // used for solving an instantaneous csp
    virtual int isSatisfied(InstantSolver &context) const = 0;

    // used by instantaneous csp to set up mappings from vars to constraints and vice versa,
    // which it can later use for GAC, etc
    virtual std::set<Variable_r> getVariables() const = 0;

    // change the domain of the specified variable in the given context to comply with this constraint
    // and the domains of the other variables involved
    // could just call the context's default propagator
    // return whatever values it has removed from the variable's domain
    virtual std::set<int> propagate(Variable &v, InstantSolver &context) = 0;

    friend bool operator< (const Constraint &lhs, const Constraint &rhs) {
        return lhs.lt(rhs);
    }
    friend bool operator== (const Constraint &lhs, const Constraint &rhs) {
        return lhs.eq(rhs);
    }

private:
    virtual bool lt(const Constraint &b) const = 0;
    virtual bool eq(const Constraint &b) const = 0;
};