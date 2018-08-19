#pragma once

#include "../Constraint.h"

class UntilConstraint : public Constraint
{
public:
    UntilConstraint(Expression &a, Expression &b);
    ~UntilConstraint();

    void normalize(std::set<Constraint_r> &constraintList,
                   std::set<Variable_r> &variableList) override;

    int isSatisfied(SearchNode &context, int time) const override;

    std::set<Variable_r> getVariables() const override;

    std::vector<std::set<int>> propagate(Variable &v, SearchNode &context) override;
private:
private:
    bool lt(const Constraint &rhs) const override;
    bool eq(const Constraint &rhs) const override;

    Expression &mExpr;
    Expression &mUntilExpr;
};