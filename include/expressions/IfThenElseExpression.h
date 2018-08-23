#pragma once

#include "../Expression.h"

class IfThenElseExpression : public Expression
{
public:
    IfThenElseExpression(Expression &ifExp, Expression &thenExp, Expression &elseExp);

    int evaluate(SearchNode &context, int time) const override;
    Expression& normalize(std::set<Constraint_r> &constraintList,
                          std::set<Variable_r> &variableList) override;
    domain_t getDomain(SearchNode &context, int time) const override;
    domain_t getInitialDomain() const override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
    Expression &mExpr3;
};