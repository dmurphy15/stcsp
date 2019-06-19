#pragma once

#include "../Constraint.h"

class NEQConstraint : public Constraint
{
public:
    NEQConstraint(Expression &a, Expression &b, int expressionSetId=-1);
    ~NEQConstraint();
    bool isSatisfied(SearchNode &context, int time) const override;
    std::map<Variable_r, std::vector<std::set<int>>> propagate(SearchNode &context) override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
    Constraint& build(std::vector<Expression_r>& expressions) override {
        return *new NEQConstraint(expressions[0], expressions[1]);
    }
};