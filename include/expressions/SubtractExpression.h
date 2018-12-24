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
                           std::set<Variable_r> &variableList) override;
    Domain getDomain(SearchNode &context, int time) const override;
    Domain getInitialDomain() const override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
};