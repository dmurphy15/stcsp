#pragma once

#include "../Expression.h"

class DivideExpression : public Expression
{
public:
    DivideExpression(Expression &a, Expression &b) : Expression({a, b}, false), mExpr1(a), mExpr2(b) {}

    int evaluate(SearchNode &context, int time) const override {
        return mExpr1.evaluate(context, time) / mExpr2.evaluate(context, time);
    }
private:
    Expression &mExpr1;
    Expression &mExpr2;
    Expression& build(std::vector<Expression_r>& expressions) override {
        return *new DivideExpression(expressions[0], expressions[1]);
    }
    int evaluateFake(const std::vector<int>& values) const override {
        return values[0] / values[1];
    }
};