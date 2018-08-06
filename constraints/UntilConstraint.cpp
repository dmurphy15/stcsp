#include "UntilConstraint.h"

#include "../Variable.h"
#include "../Expression.h"
#include "../expressions/VariableExpression.h"
#include "PrimitiveUntilConstraint.h"
#include "EqualConstraint.h"
#include "../Constraint.h"

#include "../SearchNode.h"

UntilConstraint::UntilConstraint(Expression &a, Expression &b) :
        mExpr(a),
        mUntilExpr(b) {}

UntilConstraint::~UntilConstraint() {}

void UntilConstraint::normalize(std::set<Constraint_r> &constraintList,
                                std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr1 = mExpr.normalize(constraintList, variableList);
    Expression &equivalentExpr2 = mUntilExpr.normalize(constraintList, variableList);
    Variable &var1 = *new Variable(equivalentExpr1.getInitialDomain());
    Variable &var2 = *new Variable(equivalentExpr2.getInitialDomain());
    VariableExpression &varExpr1 = *new VariableExpression(var1);
    VariableExpression &varExpr2 = *new VariableExpression(var2);
    constraintList.insert(*new EqualConstraint(varExpr1, equivalentExpr1));
    constraintList.insert(*new EqualConstraint(varExpr2, equivalentExpr2));
    Constraint &equivalentConstraint = *new PrimitiveUntilConstraint(varExpr1, varExpr2);

    variableList.insert(var1);
    variableList.insert(var2);
    constraintList.insert(equivalentConstraint);
}

int UntilConstraint::isSatisfied(SearchNode &context, int time) const
{
    return (mExpr.evaluate(context, time) != 0) || (mUntilExpr.evaluate(context, time) != 0);
}

std::set<Variable_r> UntilConstraint::getVariables() const
{
    std::set<Variable_r> vars1 = mExpr.getVariables();
    std::set<Variable_r> vars2 = mUntilExpr.getVariables();
    vars1.insert(vars2.begin(), vars2.end());
    return vars1;
}

std::vector<std::set<int>> UntilConstraint::propagate(Variable &v, SearchNode &context)
{
    return context.defaultPropagate(v, *this);
}

bool UntilConstraint::lt(const Constraint &rhs) const {
    if (typeid(*this).before(typeid(rhs))) {
        return true;
    } else if (typeid(*this) == typeid(rhs)) {
        const UntilConstraint &p = static_cast<const UntilConstraint&>(rhs);
        if (mExpr < p.mExpr) {
            return true;
        } else if (mExpr == p.mExpr) {
            return mUntilExpr < p.mUntilExpr;
        }
    }
    return false;
}

bool UntilConstraint::eq(const Constraint &rhs) const {
    if (typeid(*this) == typeid(rhs)) {
        const UntilConstraint &p = static_cast<const UntilConstraint&>(rhs);
        return (mExpr == p.mExpr) && (mUntilExpr == p.mUntilExpr);
    }
    return false;
}