#pragma once

#include "../Constraint.h"

class UntilConstraint : public Constraint
{
public:
    UntilConstraint(Expression &a, Expression &b);
    ~UntilConstraint();

    void normalize(std::set<Constraint_r> &constraintList,
                   std::set<Variable_r> &variableList) override;

    int isSatisfied(InstantSolver &context) const override;

    std::set<Variable_r> getVariables() const override;

    std::set<int> propagate(Variable &v, InstantSolver &context) override;
private:
private:
    bool lt(const Constraint &rhs) const override;
    bool eq(const Constraint &rhs) const override;

    Expression &mExpr1;
    Expression &mExpr2;
};