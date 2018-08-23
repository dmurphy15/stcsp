#include "../../include/constraints/ImplyConstraint.h"

#include "../../include/Expression.h"

#include "../../include/SearchNode.h"

ImplyConstraint::ImplyConstraint(Expression &a, Expression &b) :
        mExpr1(a),
        mExpr2(b) {}

ImplyConstraint::~ImplyConstraint() {}

void ImplyConstraint::normalize(std::set<Constraint_r> &constraintList,
                                std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr1 = mExpr1.normalize(constraintList, variableList);
    Expression &equivalentExpr2 = mExpr2.normalize(constraintList, variableList);
    constraintList.insert(*new ImplyConstraint(equivalentExpr1, equivalentExpr2));
}

int ImplyConstraint::isSatisfied(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) == 0 || mExpr2.evaluate(context, time) != 0;
}

std::set<Variable_r> ImplyConstraint::getVariables() const
{
    std::set<Variable_r> ret = mExpr1.getVariables();
    std::set<Variable_r>&& v = mExpr2.getVariables();
    ret.insert(v2.begin(), v2.end());
    return ret;
}

std::vector<std::set<int>> ImplyConstraint::propagate(Variable &v, SearchNode &context)
{
    return context.defaultPropagate(v, *this);
}

bool ImplyConstraint::lt(const Constraint &rhs) const {
    return (typeid(*this).before(typeid(rhs))) ||
           ((typeid(*this) == typeid(rhs)) &&
            (mExpr1 < static_cast<const ImplyConstraint&>(rhs).mExpr1)) ||
                ((typeid(*this) == typeid(rhs)) &&
                 (mExpr1 == static_cast<const ImplyConstraint&>(rhs).mExpr1) &&
                 (mExpr2 < static_cast<const ImplyConstraint&>(rhs).mExpr2));
}
bool ImplyConstraint::eq(const Constraint &rhs) const {
    return (typeid(*this) == typeid(rhs)) &&
           (mExpr1 == static_cast<const ImplyConstraint&>(rhs).mExpr1) &&
            (mExpr2 == static_cast<const ImplyConstraint&>(rhs).mExpr2);
}