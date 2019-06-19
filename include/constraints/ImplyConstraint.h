#pragma once

#include "../Constraint.h"

class ImplyConstraint : public Constraint
{
public:
    ImplyConstraint(Expression &a, Expression &b, int expressionSetId=-1);
    ~ImplyConstraint();
    bool isSatisfied(SearchNode &context, int time) const override;
    std::map<Variable_r, std::vector<std::set<int>>> propagate(SearchNode &context) override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
    Constraint& build(std::vector<Expression_r>& expressions) override {
        return *new ImplyConstraint(expressions[0], expressions[1]);
    }
};