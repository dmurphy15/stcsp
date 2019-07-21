#pragma once

#include "../Expression.h"

/** class to represent Next(A). evaluates to its underlying expression one timepoint in the future. Normalized
 * using PrimitiveNextConstraints.
 */

class NextExpression : public Expression
{
public:
    NextExpression(Expression &a);

    int evaluate(SearchNode &context, int time) const override;
    Expression& normalize(std::set<Constraint_r> &constraintList,
                          std::map<Expression_r, Expression_r> &normalizedMap,
                          std::set<Variable_r> &variableList) override;
    domain_t getDomain(SearchNode &context, int time) const override;
    domain_t getInitialDomain() const override;
private:
    Expression &mExpr;
};