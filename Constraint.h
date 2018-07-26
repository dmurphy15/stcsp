#pragma once

#include <set>
#include <functional>

#include "Comparators.h"

class Variable;
class Expression;
struct ExpressionLtComparator;
class InstantaneousCSP;


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
    Constraint(std::set<Expression *, ExpressionLtComparator> expressions) {
        mExpressions = expressions;
    }

    /* the consts here mean that the pointer cannot be made to point somewhere else;
     * it essentially forces you to allocate space for the set ahead of time */
    virtual void normalize(std::set<Constraint *, ConstraintLtComparator> *constraintList,
                           std::set<Variable *> *variableList) const = 0;

    // used for solving an instantaneous csp
    virtual int isSatisfied(InstantaneousCSP *context) const = 0;

    // used by instantaneous csp to set up mappings from vars to constraints and vice versa,
    // which it can later use for GAC, etc
    virtual std::set<Variable *> getVariables() const = 0;

    // change the domain of the specified variable in the given context to comply with this constraint
    // and the domains of the other variables involved
    // could just call the context's default propagator
    // return a boolean indicating whether the variable's domain changed
    virtual bool propagate(Variable *v, InstantaneousCSP *context) = 0;

//    friend bool operator< (const Constraint &lhs, const Constraint &rhs);
//    friend bool operator== (const Constraint &lhs, const Constraint &rhs);

    friend ConstraintLtComparator;
private:
    // this is just used for dominance detection; should not be used by derived classes
    std::set<Expression *, ExpressionLtComparator> mExpressions;
};