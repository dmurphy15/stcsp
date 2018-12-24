#pragma once

#include "../Expression.h"

class AddExpression : public Expression
{
public:
    AddExpression(Expression &a, Expression &b) : Expression({a, b}, true), mExpr1(a), mExpr2(b) {}
    int evaluate(SearchNode &context, int time) const override {
        return mExpr1.evaluate(context, time) + mExpr2.evaluate(context, time);
    }
    // overriding default implementations so I can take advantage of symmetry
    Domain getDomain(SearchNode &context, int time) const override;
    Domain getInitialDomain() const override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
    Expression& build(std::vector<Expression_r>& expressions) override {
        return *new AddExpression(expressions[0], expressions[1]);
    }
};