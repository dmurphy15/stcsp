#pragma once
#include <initializer_list>

#include "types.h"

/*
 * Interface that all variables and operations on variables must implement
 */
class Expression
{
public:
    Expression(std::initializer_list<Expression_r> expressions, bool symmetric);

    virtual int evaluate(SearchNode &context, int time) const = 0;

    /**
     * default implementations of these are provided, but should be overridden if:
     * 1) the expression needs special normalization (default produces no new variables/constraints)
     * 2) there is a faster way to obtain the domain of the expression (default is just brute-force) - you can almost
     *      certainly do better if dealing with a boolean or symmetric expression; for a symmetry example see
     *      AddExpression, for a boolean example see EqualExpression. In fact, the default may only be suitable for
     *      things like abs, mod, int division, etc.
     */
     // the expression returned should not be *this
    virtual Expression& normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList);
    virtual domain_t getDomain(SearchNode &context, int time) const;
    virtual domain_t getInitialDomain() const;

    // TODO give it an optional "binarize" function

    /**
     * virtual so that variableExpression and constantExpression can override it
     * !!!!! also FirstExpression is now going to override it! nice!
     */
    virtual std::set<Variable_r> getVariables(bool root) const;
    friend bool operator ==(const Expression &a, const Expression &b) {
        return a.eq(b);
    }
    friend bool operator <(const Expression &a, const Expression &b) {
        return a.lt(b);
    }
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
     * build: simply call the constructor, passing in the expressions from the vector. the returned expression should not be *this
     * evaluateFake: evaluate the expression as if the expressions in mExpressions had been assigned the corresponding
     *                  values
     *
     * these are used for the default normalize and getDomain/getInitialDomain implementations respectively. If you
     * don't use those, then no need to override these.
     */
    virtual Expression& build(std::vector<Expression_r>& expressions);
    virtual int evaluateFake(const std::vector<int>& values) const;
    /**
     * used only for comparison between expressions.
     */
    std::vector<Expression_r> mExpressions;

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
};