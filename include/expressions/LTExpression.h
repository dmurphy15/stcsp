#pragma once

#include "../Expression.h"

class LTExpression : public Expression
{
public:
    LTExpression(Expression &a, Expression &b);

    int evaluate(SearchNode &context, int time) const override;
    domain_t getDomain(SearchNode &context, int time) const override;
    domain_t getInitialDomain() const override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
    Expression& build(const std::vector<Expression_r>& expressions) override {
        return *new LTExpression(expressions[0], expressions[1]);
    }
};