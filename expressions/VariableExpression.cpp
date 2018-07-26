#include "VariableExpression.h"

#include "../Variable.h"

VariableExpression::VariableExpression(Variable *v) :
        Expression({}),
        mVariable(v) {}

int VariableExpression::evaluate(InstantaneousCSP *context) const
{
    return mVariable->evaluate(context);
}

std::set<Variable *> VariableExpression::getVariables() const
{
    return {mVariable};
}
//
//bool VariableExpression::lt(Expression *rhs) const
//{
//    return (typeid(*this).before(typeid(rhs))) ||
//            ((typeid(*this) == typeid(rhs)) &&
//             (mVariable < static_cast<const VariableExpression&>(rhs).mVariable));
//}
//
//bool VariableExpression::eq(const Expression &rhs) const
//{
//    return (typeid(*this) == typeid(rhs)) &&
//           (mVariable == static_cast<const VariableExpression&>(rhs).mVariable);
//}