#pragma once

#include "../../Expression.h"

/**
 * Class for a FirstExpression. This is special because in the very first timepoint, it should be
 * treated as if it were just another expression, but in all other timepoints, it should be treated
 * as if its underlying variables were all constants. Essentially evaluates to a constant equal to
 * the very first value that its underlying expression takes.
 *
 * The solver should swap out all FirstExpressions after solving the first SearchNode, so this expression
 * should not be present outside the root SearchNode
 */
class FirstExpression : public Expression
{
public:
    FirstExpression(Expression &a);

    int evaluate(SearchNode &context, int time) const override;
    domain_t getDomain(SearchNode &context, int time) const override;
    domain_t getInitialDomain() const override;
    bool containsFirstExpression() override;
    Expression& freezeFirstExpressions(SearchNode& rootNode) override;
private:
    Expression &mExpr;
    Expression& build(std::vector<Expression_r>& expressions) {
        return *new FirstExpression(expressions[0]);
    }
};