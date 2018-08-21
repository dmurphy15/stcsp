#pragma once

#include "../Expression.h"

class ConstantExpression;

class AtExpression : public Expression
{
public:
    AtExpression(Expression &a, ConstantExpression &b);

    int evaluate(SearchNode &context, int time) const override;

    std::set<Variable_r> getVariables() const override;

    Expression& normalize(std::set<Constraint_r> &constraintList,
                          std::set<Variable_r> &variableList) override;
    domain_t getDomain(SearchNode &context, int time) const override;
    domain_t getInitialDomain() const override;
private:
    bool lt(const Expression &rhs) const override;
    bool eq(const Expression &rhs) const override;
    Expression &mExpr1;
    ConstantExpression &mExpr2;
};