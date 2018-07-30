#pragma once

#include "../Expression.h"

class ConstantExpression : public Expression
{
    using Variable_r = std::reference_wrapper<Variable>;
public:
    ConstantExpression(int constant);

    int evaluate(InstantSolver &context) const override;

    std::set<Variable_r> getVariables() const override;
private:
    bool lt(const Expression &rhs) const override;
    bool eq(const Expression &rhs) const override;
    const int mConstant;
};