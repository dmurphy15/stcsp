#include "UntilConstraint.h"

#include "../Variable.h"
#include "../Expression.h"
#include "../expressions/VariableExpression.h"
#include "PrimitiveUntilConstraint.h"
#include "EqualConstraint.h"
#include "../Constraint.h"

#include "../InstantSolver.h"

UntilConstraint::UntilConstraint(Expression &a, Expression &b) :
        mExpr1(a),
        mExpr2(b) {}

UntilConstraint::~UntilConstraint() {}

void UntilConstraint::normalize(std::set<Constraint_r> &constraintList,
                                std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr1 = mExpr1.normalize(constraintList, variableList);
    Expression &equivalentExpr2 = mExpr2.normalize(constraintList, variableList);
    VariableExpression &varExpr1 = *new VariableExpression(*new Variable(equivalentExpr1.getInitialDomain()));
    VariableExpression &varExpr2 = *new VariableExpression(*new Variable(equivalentExpr2.getInitialDomain()));
    constraintList.insert(*new EqualConstraint(varExpr1, equivalentExpr1));
    constraintList.insert(*new EqualConstraint(varExpr2, equivalentExpr2));
    Constraint &equivalentConstraint = *new PrimitiveUntilConstraint(varExpr1, varExpr2);
    constraintList.insert(equivalentConstraint);
}

int UntilConstraint::isSatisfied(InstantSolver &context) const
{
    return (mExpr1.evaluate(context) != 0) || (mExpr2.evaluate(context) != 0);
}

std::set<Variable_r> UntilConstraint::getVariables() const
{
    std::set<Variable_r> vars1 = mExpr1.getVariables();
    std::set<Variable_r> vars2 = mExpr2.getVariables();
    vars1.insert(vars2.begin(), vars2.end());
    return vars1;
}

std::set<int> UntilConstraint::propagate(Variable &v, InstantSolver &context)
{
    return context.defaultPropagate(v, *this);
}

bool UntilConstraint::lt(const Constraint &rhs) const {
    if (typeid(*this).before(typeid(rhs))) {
        return true;
    } else if (typeid(*this) == typeid(rhs)) {
        const UntilConstraint &p = static_cast<const UntilConstraint&>(rhs);
        if (mExpr1 < p.mExpr1) {
            return true;
        } else if (mExpr1 == p.mExpr1) {
            return mExpr2 < p.mExpr2;
        }
    }
    return false;
}

bool UntilConstraint::eq(const Constraint &rhs) const {
    if (typeid(*this) == typeid(rhs)) {
        const UntilConstraint &p = static_cast<const UntilConstraint&>(rhs);
        return (mExpr1 == p.mExpr1) && (mExpr2 == p.mExpr2);
    }
    return false;
}