#include "AddExpression.h"

#include "../Variable.h"

AddExpression::AddExpression(Expression *a, Expression *b) :
        Expression({a, b}),
        mExpr1(a),
        mExpr2(b) {}

int AddExpression::evaluate(InstantaneousCSP *context) const
{
    return mExpr1->evaluate(context) + mExpr2->evaluate(context);
}

std::set<Variable *> AddExpression::getVariables() const
{
    std::set<Variable *> vars1 = mExpr1->getVariables();
    std::set<Variable *> vars2 = mExpr2->getVariables();
    vars1.insert(vars2.begin(), vars2.end());
    return vars1;
}

