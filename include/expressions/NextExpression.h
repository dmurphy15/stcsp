#pragma once

#include "../Expression.h"

class NextExpression : public Expression
{
public:
    NextExpression(Expression &a);

    int evaluate(SearchNode &context, int time) const override;
    Expression& normalize(std::set<Constraint_r> &constraintList,
                          std::set<Variable_r> &variableList) override;
    Domain getDomain(SearchNode &context, int time) const override;
    Domain getInitialDomain() const override;
private:
    Expression &mExpr;
};