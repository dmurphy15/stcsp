#include "Constraint.h"

#include "Expression.h"

Constraint::Constraint(std::set<std::reference_wrapper<Expression>> expressions) {
    mExpressions = expressions;
}

bool operator< (const Constraint &lhs, const Constraint &rhs)
{
    return typeid(lhs).before(typeid(rhs)) ||
        ((typeid(lhs) == typeid(rhs)) &&
         (lhs.mExpressions != rhs.mExpressions) &&
         (&lhs < &rhs));
}

bool operator== (const Constraint &lhs, const Constraint &rhs)
{
    return (typeid(lhs) == typeid(rhs)) && (lhs.mExpressions == rhs.mExpressions);
}