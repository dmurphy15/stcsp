#pragma once

#include "../Expression.h"

/*
 * Class for a an Expression that holds a single Variable. Used to separate
 * Expression attributes/methods from Variable attributes/methods. Also makes
 * parsing easier, so we can make multiple references to the same underlying variable.
 */
class VariableExpression : Expression
{
public:
    VariableExpression(Variable &v);

    int evaluate(InstantaneousCSP &context) const override;

    std::set<std::reference_wrapper<Variable>> getVariables() const override;

private:
    bool lt(const Expression &rhs) const;
    bool eq(const Expression &rhs) const;
    Variable &mVariable;
};