#pragma once

#include "../Expression.h"

class AddExpression : Expression
{
public:
    AddExpression(Expression &a, Expression &b);

    int evaluate(InstantaneousCSP &context) const override;

    std::set<std::reference_wrapper<Variable>> getVariables() const override;

private:
    Expression &mExpr1;
    Expression &mExpr2;
};