#include "AddExpression.h"

#include "../Variable.h"

AddExpression::AddExpression(Expression &a, Expression &b) :
        mExpr1(a),
        mExpr2(b),
        mExpressions({a, b}) {}

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
    return *new AddExpression(equivalentExpr1, equivalentExpr2);
}

std::set<int> AddExpression::getDomain(InstantSolver &context) const
{
    std::set<int> domain1 = mExpr1.getDomain(context);
    std::set<int> domain2 = mExpr2.getDomain(context);
    std::set<int> ret;
    for (int i : domain1) {
        for (int j : domain2) {
            ret.insert(i+j);
        }
    }
    return ret;
}

std::set<int> AddExpression::getInitialDomain() const
{
    std::set<int> domain1 = mExpr1.getInitialDomain();
    std::set<int> domain2 = mExpr2.getInitialDomain();
    std::set<int> ret;
    for (int i : domain1) {
        for (int j : domain2) {
            ret.insert(i+j);
        }
    }
    return ret;
}
