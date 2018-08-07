#include "MultiplyExpression.h"

#include "../Variable.h"

MultiplyExpression::MultiplyExpression(Expression &a, Expression &b) :
        mExpr1(a),
        mExpr2(b),
        mExpressions({a, b}) {}

int MultiplyExpression::evaluate(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) * mExpr2.evaluate(context, time);
}

std::set<Variable_r> MultiplyExpression::getVariables() const
{
    std::set<Variable_r> vars1 = mExpr1.getVariables();
    std::set<Variable_r> vars2 = mExpr2.getVariables();
    vars1.insert(vars2.begin(), vars2.end());
    return vars1;
}

Expression& MultiplyExpression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr1 = mExpr1.normalize(constraintList, variableList);
    Expression &equivalentExpr2 = mExpr2.normalize(constraintList, variableList);
    return *new MultiplyExpression(equivalentExpr1, equivalentExpr2);
}

std::set<int> MultiplyExpression::getDomain(SearchNode &context, int time) const
{
    std::set<int> domain1 = mExpr1.getDomain(context, time);
    std::set<int> domain2 = mExpr2.getDomain(context, time);
    std::set<int> ret;
    for (int i : domain1) {
        for (int j : domain2) {
            ret.insert(i*j);
        }
    }
    return ret;
}

std::set<int> MultiplyExpression::getInitialDomain() const
{
    std::set<int> domain1 = mExpr1.getInitialDomain();
    std::set<int> domain2 = mExpr2.getInitialDomain();
    std::set<int> ret;
    for (int i : domain1) {
        for (int j : domain2) {
            ret.insert(i*j);
        }
    }
    return ret;
}
