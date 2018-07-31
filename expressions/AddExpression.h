#pragma once

#include "../Expression.h"

class AddExpression : public Expression
{
public:
    AddExpression(Expression &a, Expression &b);

    int evaluate(InstantSolver &context) const override;

    std::set<Variable_r> getVariables() const override;

    Expression& normalize(std::set<Constraint_r> &constraintList,
                           std::set<Variable_r> &variableList) override;

private:
    Expression &mExpr1;
    Expression &mExpr2;
};