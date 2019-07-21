#pragma once

#include <set>
#include <functional>

#include "types.h"

/***************************** Constraint ******************************************************************************
 * A Constraint constrains the values that certain expressions can take, and through them it constrains the assignments
 * that can be given to underlying variables.
 * Handles things like normalizing the constraint and evaluating whether it is satisfied when its underlying variables
 * are given certain values.
 * ********************************************************************************************************************/

class Constraint
{
public:
    /**
     * Create a new constraint
     * @param expressions - the constrained expressions
     * @param symmetric - whether or not the order of the constrained expressions matters (helps detect when
     *  two constraints are equivalent)
     * @param expressionSetId - a unique id for this list of expressions, or -1 if we want it to look up an id
     */
    Constraint(std::initializer_list<Expression_r> expressions, bool symmetric, int expressionSetId=-1);

    /**
     * places a normalized version of this constraint in the constraintList
     * a default implementation is provided; should be overridden if you're making a constraint with special
     * normalization rules
     * if you don't override this, then you should implement the build method
     * @param constraintList - the list in which to insert the normalized constraint
     * @param normalizedMap - a map of expressions to their normalized versions, to avoid creating too many
     *  auxiliary variables by normalizing equivalent expressions
     * @param variableList - a list of variables, into which auxiliary variables will be placed
     */
    virtual void normalize(std::set<Constraint_r> &constraintList,
                           std::map<Expression_r, Expression_r> &normalizedMap,
                           std::set<Variable_r> &variableList);

    /**
     * is the constraint satisfied at a certain timepoint within a certain searchnode
     * @param context - the searchnode we're looking at
     * @param time - the timepoint within the searchnode
     * @return whether the constraint is satisfied
     */
    virtual bool isSatisfied(SearchNode &context, int time) const = 0;

    /** get the underlying variables constrained by this constraint */
    std::set<Variable_r> getVariables() const {
        return mVariables;
    };

    /**
     * change the domain of the variables in the given context to comply with this constraint (via a consistency
     * algorithm)
     * could just call the context's default propagator
     * ***NOTE that every vector in the map we return MUST have a length equal to prefixK
     * @return whatever values we have removed from the variable's domains */
    virtual std::map<Variable_r, std::vector<std::set<int>>> propagate(SearchNode &context) = 0;

    friend bool operator< (const Constraint &lhs, const Constraint &rhs);
    friend bool operator== (const Constraint &lhs, const Constraint &rhs);

    int getExpressionSetId() { return mExpressionSetId; };
    /** return whether this constraint constrains a FirstExpression, either directly or
     * indirectly through other expressions */
    bool containsFirstExpression();
    /** return an equivalent constraint, except where all firstExpressions have been frozen to ConstantExpressions
     * whose values match the values they took in the root node */
    Constraint& freezeFirstExpressions();
private:
    /** the expressions that this constraint directly constrains */
    std::vector<Expression_r> mExpressions;
    /** implemented by derived classes, to allow us to build a new, equivalent member of the derived class using
     * our expressions */
    virtual Constraint& build(std::vector<Expression_r>& expressions);
    int mExpressionSetId;
    bool mContainsFirstExpression = true; // don't use this directly; call containsFirstExpression()
    /** the variables that this constraint ultimately constrains */
    std::set<Variable_r> mVariables;

    /** helper function for gathering all our variables */
    std::set<Variable_r> _getVariables() const;
};