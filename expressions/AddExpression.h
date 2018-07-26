#pragma once

#include "../Expression.h"

class AddExpression : public Expression
{
public:
    AddExpression(Expression *a, Expression *b);

    int evaluate(InstantaneousCSP *context) const override;

    std::set<Variable *> getVariables() const override;

private:
    Expression *mExpr1;
    Expression *mExpr2;
};