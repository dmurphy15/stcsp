#include "ConstantExpression.h"


ConstantExpression::ConstantExpression(int constant) : mConstant(constant) {}

int ConstantExpression::evaluate(int time) const
{
    return mConstant;
}

std::unordered_set<Variable *> ConstantExpression::getVariables() const
{
    return {};
}