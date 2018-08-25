#pragma once

#include "../Constraint.h"

class NEQConstraint : public Constraint
{
public:
    NEQConstraint(Expression &a, Expression &b);
    ~NEQConstraint();
    bool isSatisfied(SearchNode &context, int time) const override;
    std::vector<std::set<int>> propagate(Variable &v, SearchNode &context) override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
    Constraint& build(std::vector<Expression_r>& expressions) override {
        return *new NEQConstraint(expressions[0], expressions[1]);
    }
};