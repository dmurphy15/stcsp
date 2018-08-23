#include "../../include/expressions/MultiplyExpression.h"

#include "../../include/Variable.h"

MultiplyExpression::MultiplyExpression(Expression &a, Expression &b) :
        Expression({a, b}, true),
        mExpr1(a),
        mExpr2(b) {}

int MultiplyExpression::evaluate(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) * mExpr2.evaluate(context, time);
}

Expression& MultiplyExpression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr1 = mExpr1.normalize(constraintList, variableList);
    Expression &equivalentExpr2 = mExpr2.normalize(constraintList, variableList);
    return *new MultiplyExpression(equivalentExpr1, equivalentExpr2);
}

domain_t MultiplyExpression::getDomain(SearchNode &context, int time) const
{
    domain_t domain1 = mExpr1.getDomain(context, time);
    domain_t domain2 = mExpr2.getDomain(context, time);
    domain_t ret;
    for (int i : domain1) {
        for (int j : domain2) {
            ret.insert(i*j);
        }
    }
    return ret;
}

domain_t MultiplyExpression::getInitialDomain() const
{
    domain_t domain1 = mExpr1.getInitialDomain();
    domain_t domain2 = mExpr2.getInitialDomain();
    domain_t ret;
    for (int i : domain1) {
        for (int j : domain2) {
            ret.insert(i*j);
        }
    }
    return ret;
}
