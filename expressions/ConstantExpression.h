#pragma once

#include "../Expression.h"

class ConstantExpression : Expression
{
public:
    ConstantExpression(int constant);

    int evaluate(InstantaneousCSP &context) const override;

    std::set<std::reference_wrapper<Variable>> getVariables() const override;
private:
    bool lt(const Expression &rhs) const;
    bool eq(const Expression &rhs) const;
    const int mConstant;
};