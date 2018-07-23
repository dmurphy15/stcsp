#pragma once

#include "../Constraint.h"

class EqualConstraint : Constraint
{
public:
    EqualConstraint(Expression &a, Expression &b);
    ~EqualConstraint();

    void normalize(std::set<std::reference_wrapper<Constraint>> &constraintList,
                   std::set<std::reference_wrapper<Variable>> &variableList) const override;

    int isSatisfied(InstantaneousCSP &context) const override;

    std::set<std::reference_wrapper<Variable>> getVariables() const override;

    bool propagate(Variable &v, InstantaneousCSP &context) const override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
};