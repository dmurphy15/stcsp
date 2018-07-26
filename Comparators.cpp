#include "Comparators.h"

#include "Expression.h"
#include "Constraint.h"

bool ExpressionLtComparator::operator()(Expression *lhs, Expression *rhs) {
    return typeid(*lhs).before(typeid(*rhs)) ||
           ((typeid(*lhs) == typeid(*rhs)) && (lhs->mExpressions != rhs->mExpressions) && (lhs < rhs));
}

bool ConstraintLtComparator::operator()(Constraint *lhs, Constraint *rhs) {
    return typeid(*lhs).before(typeid(*rhs)) ||
           ((typeid(*lhs) == typeid(*rhs)) &&
            (lhs->mExpressions != rhs->mExpressions) &&
            (lhs < rhs));
}