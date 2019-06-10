#pragma once

#include "../Constraint.h"

class UntilConstraint : public Constraint
{
public:
    UntilConstraint(Expression &a, Expression &b, int expressionSetId=-1);
    ~UntilConstraint();

    void normalize(std::set<Constraint_r> &constraintList,
                   std::set<Variable_r> &variableList) override;

    bool isSatisfied(SearchNode &context, int time) const override;
    std::vector<std::set<int>> propagate(Variable &v, SearchNode &context) override;
private:
    Expression &mExpr;
    Expression &mUntilExpr;
};