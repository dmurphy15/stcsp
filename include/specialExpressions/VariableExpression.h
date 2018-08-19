#pragma once

#include "../Expression.h"

/*
 * Class for a an Expression that holds a single Variable. Used to separate
 * Expression attributes/methods from Variable attributes/methods. Also makes
 * parsing easier, so we can make multiple references to the same underlying variable.
 */
class VariableExpression : public Expression
{
public:
    VariableExpression(Variable &v);

    int evaluate(SearchNode &context, int time) const override;

    std::set<Variable_r> getVariables() const override;

    Expression& normalize(std::set<Constraint_r> &constraintList,
                   std::set<Variable_r> &variableList) override;
    std::set<int> getDomain(SearchNode &context, int time) const override;
    std::set<int> getInitialDomain() const override;
private:
    bool lt(const Expression &rhs) const override;
    bool eq(const Expression &rhs) const override;
    Variable &mVariable;
};