#include "ConstantExpression.h"


ConstantExpression::ConstantExpression(int constant) :
        Expression(std::set<std::reference_wrapper<Expression>>{}),
        mConstant(constant) {}

int ConstantExpression::evaluate(InstantaneousCSP &context) const
{
    return mConstant;
}

std::set<std::reference_wrapper<Variable>> ConstantExpression::getVariables() const
{
    return {};
}

bool ConstantExpression::lt(const Expression &rhs) const
{
    return (typeid(*this).before(typeid(rhs))) ||
           ((typeid(*this) == typeid(rhs)) &&
            (mConstant < static_cast<const ConstantExpression&>(rhs).mConstant));
}

bool ConstantExpression::eq(const Expression &rhs) const
{
    return (typeid(*this) == typeid(rhs)) &&
           (mConstant == static_cast<const ConstantExpression&>(rhs).mConstant);
}