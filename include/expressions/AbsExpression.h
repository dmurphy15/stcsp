#pragma once

#include "../Expression.h"

class AbsExpression : public Expression
{
public:
    AbsExpression(Expression &a) : Expression({a}, false), mExpr1(a) {}

    int evaluate(SearchNode &context, int time) const override
    {
        int val = mExpr1.evaluate(context, time);
        return val < 0 ? (-1 * val) : val;
    }
private:
    Expression &mExpr1;
    Expression& build(const std::vector<Expression_r>& expressions) override {
        return *new AbsExpression(expressions[0]);
    }
    int evaluateFake(const std::vector<int>& values) const override {
        int val = values[0];
        return val < 0 ? (-1 * val) : val;
    }
};