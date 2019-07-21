#pragma once

#include "../Expression.h"

class SubtractExpression : public Expression
{
public:
    SubtractExpression(Expression &a, Expression &b);

    int evaluate(SearchNode &context, int time) const override;
    /**
     * I'm implementing this to be normalized into (a + (-1 * b)), because that may
     * actually allow us to take advantage of the symmetry of AddExpressions (if we're dealing with negative
     * numbers)
     */
    Expression& normalize(std::set<Constraint_r> &constraintList,
                           std::map<Expression_r, Expression_r> &normalizedMap,
                           std::set<Variable_r> &variableList) override;
    domain_t getDomain(SearchNode &context, int time) const override;
    domain_t getInitialDomain() const override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
};