#pragma once

#include "../Expression.h"

class LEExpression : public Expression
{
public:
    LEExpression(Expression &a, Expression &b);

    int evaluate(SearchNode &context, int time) const override;
    domain_t getDomain(SearchNode &context, int time) const override;
    domain_t getInitialDomain() const override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
    Expression& build(const std::vector<Expression_r>& expressions) override {
        return *new LEExpression(expressions[0], expressions[1]);
    }
};