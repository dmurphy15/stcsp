#pragma once

#include "../Constraint.h"

class LTConstraint : public Constraint
{
public:
    LTConstraint(Expression &a, Expression &b, int expressionSetId=-1);
    ~LTConstraint();
    bool isSatisfied(SearchNode &context, int time) const override;
    std::vector<std::set<int>> propagate(Variable &v, SearchNode &context) override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
    Constraint& build(std::vector<Expression_r>& expressions) override {
        return *new LTConstraint(expressions[0], expressions[1]);
    }
};