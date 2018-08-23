#pragma once

#include "../Expression.h"

class IfThenElseExpression : public Expression
{
public:
    IfThenElseExpression(Expression &ifExp, Expression &thenExp, Expression &elseExp);

    int evaluate(SearchNode &context, int time) const override;
    domain_t getDomain(SearchNode &context, int time) const override;
    domain_t getInitialDomain() const override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
    Expression &mExpr3;
    Expression& build(std::vector<Expression_r>& expressions) override {
        return *new IfThenElseExpression(expressions[0], expressions[1], expressions[2]);
    }
};