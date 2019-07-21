#pragma once

#include "../../Expression.h"

class FirstExpression : public Expression
{
public:
    FirstExpression(Expression &a);

    int evaluate(SearchNode &context, int time) const override;
    domain_t getDomain(SearchNode &context, int time) const override;
    domain_t getInitialDomain() const override;
    bool containsFirstExpression() override;
    Expression& freezeFirstExpressions() override;
    Expression& build(std::vector<Expression_r>& expressions) {
        return *new FirstExpression(expressions[0]);
    }
private:
    Expression &mExpr;
};