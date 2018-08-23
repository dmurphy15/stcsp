#include "../../../include/expressions/specialExpressions/ConstantExpression.h"


ConstantExpression::ConstantExpression(int constant) :
        Expression({}, false),
        mConstant(constant) {}

int ConstantExpression::evaluate(SearchNode &context, int time) const
{
    return mConstant;
}

std::set<std::reference_wrapper<Variable>> ConstantExpression::getVariables() const
{
    return {};
}

domain_t ConstantExpression::getDomain(SearchNode &context, int time) const
{
    return {mConstant};
}

domain_t ConstantExpression::getInitialDomain() const
{
    return {mConstant};
}

