#pragma once

#include "../Expression.h"

class ConstantExpression : public Expression
{
public:
    ConstantExpression(int constant);

    int evaluate(InstantSolver &context) const override;

    std::set<Variable_r> getVariables() const override;

    Expression& normalize(std::set<Constraint_r> &constraintList,
                   std::set<Variable_r> &variableList) override;
private:
    bool lt(const Expression &rhs) const override;
    bool eq(const Expression &rhs) const override;
    const int mConstant;
};