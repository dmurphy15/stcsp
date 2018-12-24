#pragma once

#include "../Expression.h"

class ConstantExpression;

class AtExpression : public Expression
{
public:
    AtExpression(Expression &a, ConstantExpression &b);

    int evaluate(SearchNode &context, int time) const override;
    Expression& normalize(std::set<Constraint_r> &constraintList,
                          std::set<Variable_r> &variableList) override;
    Domain getDomain(SearchNode &context, int time) const override;
    Domain getInitialDomain() const override;
private:
    Expression &mExpr1;
    ConstantExpression &mExpr2;
};