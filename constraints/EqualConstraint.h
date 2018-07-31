#pragma once

#include "../Constraint.h"

class EqualConstraint : public Constraint
{
public:
    EqualConstraint(Expression &a, Expression &b);
    ~EqualConstraint();

    void normalize(std::set<Constraint_r> &constraintList,
                   std::set<Variable_r> &variableList) override;

    int isSatisfied(InstantSolver &context) const override;

    std::set<Variable_r> getVariables() const override;

    std::vector<int> propagate(Variable &v, InstantSolver &context) override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
};