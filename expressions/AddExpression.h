#pragma once

#include <unordered_set>

#include "../Expression.h"

class Variable;

class AddExpression : Expression
{
public:
    AddExpression(Expression *a, Expression *b);

    int evaluate(int time) const override;

    std::unordered_set<Variable *> getVariables() const override;

private:
    Expression *mExpr1;
    Expression *mExpr2;
};