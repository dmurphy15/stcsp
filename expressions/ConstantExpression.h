#pragma once
#include <vector>

#include "../Expression.h"

class Variable;

class ConstantExpression : Expression
{
public:
    ConstantExpression(int constant);

    int evaluate(int time) const override;

    std::unordered_set<Variable *> getVariables() const override;
private:
    const int mConstant;
};