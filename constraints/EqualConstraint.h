#pragma once

#include "../Constraint.h"

class EqualConstraint : public Constraint
{
    using Variable_r = std::reference_wrapper<Variable>;
public:
    EqualConstraint(Expression &a, Expression &b);
    ~EqualConstraint();

    void normalize(std::set<Constraint_r> &constraintList,
                   std::set<Variable_r> &variableList) const override;

    int isSatisfied(InstantaneousCSP &context) const override;

    std::set<Variable_r> getVariables() const override;

    std::vector<int> propagate(Variable &v, InstantaneousCSP &context) override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
};