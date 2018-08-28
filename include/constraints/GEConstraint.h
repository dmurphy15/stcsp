#pragma once

#include "../Constraint.h"

class GEConstraint : public Constraint
{
public:
    GEConstraint(Expression &a, Expression &b);
    ~GEConstraint();
    bool isSatisfied(SearchNode &context, int time) const override;
    std::vector<std::set<int>> propagate(Variable &v, SearchNode &context) override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
    Constraint& build(std::vector<Expression_r>& expressions) override {
        return *new GEConstraint(expressions[0], expressions[1]);
    }
};