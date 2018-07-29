#pragma once

#include "../Expression.h"

class AddExpression : public Expression
{
    using Variable_r = std::reference_wrapper<Variable>;
public:
    AddExpression(Expression &a, Expression &b);

    int evaluate(InstantaneousCSP &context) const override;

    std::set<Variable_r> getVariables() const override;

private:
    Expression &mExpr1;
    Expression &mExpr2;
};