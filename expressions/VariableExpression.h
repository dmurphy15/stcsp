#pragma once
#include <unordered_set>

#include "../Expression.h"

class Variable;

/*
 * Class for a an Expression that holds a single Variable. Used to separate
 * Expression attributes/methods from Variable attributes/methods. Also makes
 * parsing easier, so we can make multiple references to the same underlying variable.
 */
class VariableExpression : Expression
{
public:
    VariableExpression(Variable *v);

    int evaluate(int time) const override;

    std::unordered_set<Variable *> getVariables() const override;

private:
    Variable *mVariable;
};