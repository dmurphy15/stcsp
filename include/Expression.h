#pragma once
#include <initializer_list>

#include "types.h"

/***************************** Expression ******************************************************************************
 * Constraints operate on Expressions, which further relate subexressions or variables.
 * Handles things like normalizing the expression and evaluating the value of the expression once its underlying
 * variables have been assigned values.
 * ********************************************************************************************************************/


class Expression
{
public:
    /**
     * Create a new expression
     * @param expressions - the directly underlying expressions
     * @param symmetric - whether or not the order of the expressions matters (helps detect when
     *  two expressions are equivalent)
     */
    Expression(std::initializer_list<Expression_r> expressions, bool symmetric);

    /**
     * get the value of this expression in a certain searchnode at a certain timepoint after its underlying variables
     *  have been assigned values
     * @param context - the searchnode within which we're evaluating
     * @param time - the time within the searchnode at which we're evaluating
     * @return the value of the expression
     */
    virtual int evaluate(SearchNode &context, int time) const = 0;

    /**
     * return a normalized equivalent expression to this one.
     * NOTE: the expression returned should not be *this
     * default implementations of this is provided, but should be overridden if:
     *   the expression needs special normalization (default produces no new variables/constraints)
     * @param constraintList - a list into which any new constraints should be placed
     * @param normalizedMap - a map from expressions to their normalized equivalents; if this expression is already in
     *  the map, we should just return what it maps to, to avoid normalizing this expression multiple times and
     *  potentially producing redundant auxiliary variables
     * @param variableList - a list into which any new variables should be placed
     * @return the equivalent, normalized version
     */
    virtual Expression& normalize(std::set<Constraint_r> &constraintList,
                                  std::map<Expression_r, Expression_r> &normalizedMap,
                                  std::set<Variable_r> &variableList);
    /**
     * get the domain of this expression within a specified searchnode at a certain timepoint
     * default implementations of this is provided, but should be overridden if:
     *   there is a faster way to obtain the domain of the expression (default is just brute-force) - you can almost
     * certainly do better if dealing with a boolean or symmetric expression; for a symmetry example see
     * AddExpression, for a boolean example see EqualExpression. In fact, the default may only be suitable for
     * things like abs, mod, int division, etc.
     * @param context - the searchnode in which we are computing our domain
     * @param time - the timepoint within the searchnode we are looking at
     * @return the domain of this expression
     */
    virtual domain_t getDomain(SearchNode &context, int time) const;
    /** do the same as above, but rather than computing our domain within the context of a searchnode (which may have
     * pruned the domains of some variables), compute our domain using the original, unpruned domains */
    virtual domain_t getInitialDomain() const;

    /** get the underlying variables. overridden by VariableExpression and ConstantExpression */
    virtual std::set<Variable_r> getVariables() const;
    friend bool operator ==(const Expression &a, const Expression &b) {
        return a.eq(b);
    }
    friend bool operator <(const Expression &a, const Expression &b) {
        return a.lt(b);
    }
    /** return an equivalent expression, except where all underlying firstExpressions have been frozen to ConstantExpressions
     * whose values match the values they took in the root node. overridden by FirstExpression */
    virtual Expression& freezeFirstExpressions(SearchNode& rootNode);
    /** return whether this expression contains a FirstExpression, either directly or
    * indirectly through other expressions */
    virtual bool containsFirstExpression();
private:
    /**
     * these are virtual so that variableExpression and constantExpression can override them
     */
    virtual bool eq(const Expression &b) const {
        return (this==&b) || (typeid(*this) == typeid(b) && mExpressions == b.mExpressions);
    }
    virtual bool lt(const Expression &b) const {
        return (this != &b) && ((typeid(*this).before(typeid(b))) || (typeid(*this) == typeid(b) && mExpressions < b.mExpressions));
    }
    /**
     * simply call the constructor, passing in the expressions from the vector.
     * the returned expression should not be *this
     *
     * this is used for the default normalize and freezeFirstExpressions implementations. If you
     * don't use the default normalize AND you know that your expression will either never contain underlying
     * FirstExpressions or will normalized away before the first searchnode is solved,
     * then no nee to override it.
     *
     * Maybe I should force all derived classes to override it...
     */
    virtual Expression& build(std::vector<Expression_r>& expressions);


     /** evaluate the expression as if the expressions in mExpressions had been assigned the corresponding values
     *
     * this is used for the default getDomain/getInitialDomain implementations. If you
     * don't use those, then no need to override it.
     */
    virtual int evaluateFake(const std::vector<int>& values) const;
    /**
     * used for comparison between expressions.
     */
    std::vector<Expression_r> mExpressions;

    /** default helpers for getDomain/getInitialDomain. they use brute-force. */
    void getDomainHelper(const std::vector<Expression_r>& expressions,
                         SearchNode &context,
                         int time,
                         std::vector<int>& values,
                         std::size_t index,
                         domain_t& accumulator) const;
    void getInitialDomainHelper(const std::vector<Expression_r>& expressions,
                                std::vector<int>& values,
                                std::size_t index,
                                domain_t& accumulator) const;

    bool mContainsFirstExpression = true;// don't use this directly; call containsFirstExpression
};