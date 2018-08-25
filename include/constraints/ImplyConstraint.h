#pragma once

#include "../Constraint.h"

class ImplyConstraint : public Constraint
{
public:
    ImplyConstraint(Expression &a, Expression &b);
    ~ImplyConstraint();
    bool isSatisfied(SearchNode &context, int time) const override;
    std::vector<std::set<int>> propagate(Variable &v, SearchNode &context) override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
    Constraint& build(std::vector<Expression_r>& expressions) override {
        return *new ImplyConstraint(expressions[0], expressions[1]);
    }
};