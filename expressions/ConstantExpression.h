#pragma once

#include "../Expression.h"

class ConstantExpression : public Expression
{
public:
    ConstantExpression(int constant);

    int evaluate(InstantaneousCSP *context) const override;

    std::set<Variable *> getVariables() const override;
private:
//    bool lt(const Expression &rhs) const;
//    bool eq(const Expression &rhs) const;
    const int mConstant;
};