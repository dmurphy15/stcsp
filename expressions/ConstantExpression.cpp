#include "ConstantExpression.h"


ConstantExpression::ConstantExpression(int constant) :
        mConstant(constant) {}

int ConstantExpression::evaluate(SearchNode &context, int time) const
{
    return mConstant;
}

std::set<std::reference_wrapper<Variable>> ConstantExpression::getVariables() const
{
    return {};
}

Expression& ConstantExpression::normalize(std::set<Constraint_r> &constraintList,
                                   std::set<Variable_r> &variableList)
{
    return *this;
}

std::set<int> ConstantExpression::getDomain(SearchNode &context, int time) const
{
    return {mConstant};
}

std::set<int> ConstantExpression::getInitialDomain() const
{
    return {mConstant};
}