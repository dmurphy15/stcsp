#include "AddExpression.h"

AddExpression::AddExpression(Expression *a, Expression *b) : mExpr1(a), mExpr2(b) {}

int AddExpression::evaluate(int time) const
{
    return mExpr1->evaluate(time) + mExpr2->evaluate(time);
}

std::unordered_set<Variable *> AddExpression::getVariables() const
{
    std::unordered_set<Variable *> vars1 = mExpr1->getVariables();
    std::unordered_set<Variable *> vars2 = mExpr2->getVariables();
    vars1.insert(vars2.begin(), vars2.end());
    return vars1;
}

