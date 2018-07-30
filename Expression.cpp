#include "Expression.h"

Expression::Expression(std::set<Expression_r> expressions) {
    mExpressions = expressions;
}

bool operator< (const Expression &lhs, const Expression &rhs) {
    return lhs.lt(rhs);
}

bool operator== (const Expression &lhs, const Expression &rhs) {
    return lhs.eq(rhs);
}

bool Expression::lt(const Expression &rhs) const {
    return typeid(*this).before(typeid(rhs)) ||
            ((typeid(*this) == typeid(rhs)) && (mExpressions != rhs.mExpressions) && (this < &rhs));
}

bool Expression::eq(const Expression &rhs) const {
    return (typeid(*this) == typeid(rhs)) && (mExpressions == rhs.mExpressions);
}