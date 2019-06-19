#pragma once

#include "../Constraint.h"

class GTConstraint : public Constraint
{
public:
    GTConstraint(Expression &a, Expression &b, int expressionSetId=-1);
    ~GTConstraint();
    bool isSatisfied(SearchNode &context, int time) const override;
    std::map<Variable_r, std::vector<std::set<int>>> propagate(SearchNode &context) override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
    Constraint& build(std::vector<Expression_r>& expressions) override {
        return *new GTConstraint(expressions[0], expressions[1]);
    }
};