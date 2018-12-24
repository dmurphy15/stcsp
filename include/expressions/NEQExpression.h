#pragma once

#include "../Expression.h"

class NEQExpression : public Expression
{
public:
    NEQExpression(Expression &a, Expression &b);

    int evaluate(SearchNode &context, int time) const override;
    Domain getDomain(SearchNode &context, int time) const override;
    Domain getInitialDomain() const override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
    Expression& build(std::vector<Expression_r>& expressions) override {
        return *new NEQExpression(expressions[0], expressions[1]);
    }
};