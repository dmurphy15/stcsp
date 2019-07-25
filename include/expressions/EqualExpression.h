// I only really made this to test how easy it would be to create new expressions; this is essentially
// almost exactly the same as an AddExpression, so it's pretty easy! If you want to delete this feel free
// since it would be really easy to reproduce based on the AddExpression

#pragma once

#include "../Expression.h"

class EqualExpression : public Expression
{
public:
    EqualExpression(Expression &a, Expression &b);

    int evaluate(SearchNode &context, int time) const override;
    domain_t getDomain(SearchNode &context, int time) const override;
    domain_t getInitialDomain() const override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
    Expression& build(const std::vector<Expression_r>& expressions) override {
        return *new EqualExpression(expressions[0], expressions[1]);
    }
};