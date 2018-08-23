#include "../../include/expressions/AbsExpression.h"

#include "../../include/Variable.h"

AbsExpression::AbsExpression(Expression &a) :
        Expression({a}, false),
        mExpr1(a) {}

int AbsExpression::evaluate(SearchNode &context, int time) const
{
    int val = mExpr1.evaluate(context, time);
    return val < 0 ? (-1 * val) : val;
}

Expression& AbsExpression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList)
{
    return *new AbsExpression(mExpr1.normalize(constraintList, variableList));
}

domain_t AbsExpression::getDomain(SearchNode &context, int time) const
{
    domain_t domain1 = mExpr1.getDomain(context, time);
    domain_t ret;
    for (int i : domain1) {
        ret.insert(i < 0 ? (-1 * i) : i);
    }
    return ret;
}

domain_t AbsExpression::getInitialDomain() const
{
    domain_t domain1 = mExpr1.getInitialDomain();
    domain_t ret;
    for (int i : domain1) {
        ret.insert(i < 0 ? (-1 * i) : i);
    }
    return ret;
}
