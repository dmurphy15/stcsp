#pragma once

#include "../Expression.h"

class AbsExpression : public Expression
{
public:
    AbsExpression(Expression &a);

    int evaluate(SearchNode &context, int time) const override;
    Expression& normalize(std::set<Constraint_r> &constraintList,
                           std::set<Variable_r> &variableList) override;
    domain_t getDomain(SearchNode &context, int time) const override;
    domain_t getInitialDomain() const override;
private:
    Expression &mExpr1;
};