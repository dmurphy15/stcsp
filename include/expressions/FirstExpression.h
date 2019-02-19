#pragma once

#include "../Expression.h"

class FirstExpression : public Expression
{
public:
    FirstExpression(Expression &a);

    int evaluate(SearchNode &context, int time) const override;
    domain_t getDomain(SearchNode &context, int time) const override;
    domain_t getInitialDomain() const override;
    //!!!!!!!!!!!!!!WARNING: I'm also only allowing this to be implemented in this way since
    void getVariables(std::set<Variable_r>& variables, bool root=true) const override;
private:
    Expression &mExpr;
    Expression& build(std::vector<Expression_r>& expressions) override {
        return *new FirstExpression(expressions[0]);
    }
};