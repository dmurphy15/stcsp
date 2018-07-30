#include "ConstantExpression.h"


ConstantExpression::ConstantExpression(int constant) :
        Expression(std::set<Expression_r>()),
        mConstant(constant) {}

int ConstantExpression::evaluate(InstantSolver &context) const
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