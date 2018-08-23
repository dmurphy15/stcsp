#pragma once

#include "../../Constraint.h"

class EqualConstraint : public Constraint
{
public:
    EqualConstraint(Expression &a, Expression &b);
    ~EqualConstraint();

    void normalize(std::set<Constraint_r> &constraintList,
                   std::set<Variable_r> &variableList) override;

    int isSatisfied(SearchNode &context, int time) const override;

    std::set<Variable_r> getVariables() const override;

    std::vector<std::set<int>> propagate(Variable &v, SearchNode &context) override;
private:
    bool lt(const Constraint &rhs) const override;
    bool eq(const Constraint &rhs) const override;

    Expression &mExpr1;
    Expression &mExpr2;
    // used to speed up comparison, since this expression is symmetric
    std::set<Expression_r> mExpressions;
};