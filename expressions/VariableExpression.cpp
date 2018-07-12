#include "VariableExpression.h"

#include "../Variable.h"

VariableExpression::VariableExpression(Variable *v) : mVariable(v) {}

int VariableExpression::evaluate(int time) const
{
    return mVariable->evaluate(time);
}

std::unordered_set<Variable *> VariableExpression::getVariables() const
{
    return {mVariable};
}