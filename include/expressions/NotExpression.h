#pragma once

#include "../Expression.h"

class NotExpression : public Expression
{
public:
    NotExpression(Expression &a);

    int evaluate(SearchNode &context, int time) const override;
    domain_t getDomain(SearchNode &context, int time) const override;
    domain_t getInitialDomain() const override;
private:
    Expression &mExpr1;
    Expression& build(std::vector<Expression_r>& expressions) override {
        return *new NotExpression(expressions[0]);
    }
};