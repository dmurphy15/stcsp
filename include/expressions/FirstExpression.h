#pragma once

#include "../Expression.h"

class FirstExpression : public Expression
{
public:
    FirstExpression(Expression &a);

    int evaluate(SearchNode &context, int time) const override;
    domain_t getDomain(SearchNode &context, int time) const override;
    domain_t getInitialDomain() const override;
    //!!!!!!!!!!!!!!WARNING: I'm also only allowing this to be implemented in this way since the root node is
    // always given a valid assignment as we progress to our current node
    std::set<Variable_r> getVariables(bool root) const override;
private:
    Expression &mExpr;
    Expression& build(std::vector<Expression_r>& expressions) override {
        return *new FirstExpression(expressions[0]);
    }
};