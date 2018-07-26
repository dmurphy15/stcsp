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
    VariableExpression(Variable *v);

    int evaluate(InstantaneousCSP *context) const override;

    std::set<Variable *> getVariables() const override;

//    bool lt(Expression *rhs) const override;
private:
//    bool eq(const Expression &rhs) const;
    Variable *mVariable;
};