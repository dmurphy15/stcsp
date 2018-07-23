#include "Expression.h"

Expression::Expression(std::set<std::reference_wrapper<Expression>> expressions) {
    mExpressions = expressions;
}

bool operator< (const Expression &lhs, const Expression &rhs) {
    return lhs.lt(rhs);
}

bool operator== (const Expression &lhs, const Expression &rhs) {
    return lhs.eq(rhs);
}

bool Expression::lt(const Expression &rhs) const {
    const Expression &lhs = *this;
    return typeid(lhs).before(typeid(rhs)) ||
    ((typeid(lhs) == typeid(rhs)) &&
     (lhs.mExpressions != rhs.mExpressions) &&
     (&lhs < &rhs));
}

bool Expression::eq(const Expression &rhs) const {
    const Expression &lhs = *this;
    return (typeid(lhs) == typeid(rhs)) && (lhs.mExpressions == rhs.mExpressions);
}