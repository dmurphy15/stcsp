#include "AddExpression.h"

#include "../Variable.h"

AddExpression::AddExpression(Expression &a, Expression &b) :
        Expression({a, b}),
        mExpr1(a),
        mExpr2(b) {}

int AddExpression::evaluate(InstantSolver &context) const
{
    return mExpr1.evaluate(context) + mExpr2.evaluate(context);
}

std::set<Variable_r> AddExpression::getVariables() const
{
    std::set<Variable_r> vars1 = mExpr1.getVariables();
    std::set<Variable_r> vars2 = mExpr2.getVariables();
    vars1.insert(vars2.begin(), vars2.end());
    return vars1;
}

Expression& AddExpression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr1 = mExpr1.normalize(constraintList, variableList);
    Expression &equivalentExpr2 = mExpr2.normalize(constraintList, variableList);
    return *new AddExpression(equivalentExpr1, equivalentExpr1);
}

