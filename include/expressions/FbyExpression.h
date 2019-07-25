#pragma once

#include "../Expression.h"

/**
 * Expression to represent "A followed by B". Effectively equivalent to C where: First(C) == First A, Next(C) == B
 * Gets normalized in essentially that way.
 */

class FbyExpression : public Expression
{
public:
    FbyExpression(Expression &a, Expression &b);

    int evaluate(SearchNode &context, int time) const override;
    Expression& normalize(std::set<Constraint_r> &constraintList,
                          std::map<Expression_r, Expression_r> &normalizedMap,
                          std::set<Variable_r> &variableList) override;
    domain_t getDomain(SearchNode &context, int time) const override;
    domain_t getInitialDomain() const override;
private:
    Expression &mExpr1;
    Expression &mExpr2;
    Expression& build(const std::vector<Expression_r>& expressions) override {
        return *new FbyExpression(expressions[0], expressions[1]);
    }
};