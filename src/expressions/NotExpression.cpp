#include "../../include/expressions/NotExpression.h"

#include "../../include/Variable.h"

NotExpression::NotExpression(Expression &a) :
        Expression({a}, false),
        mExpr1(a) {}

int NotExpression::evaluate(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) != 0 ? 0 : 1;
}

Expression& NotExpression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList)
{
    return *new NotExpression(mExpr1.normalize(constraintList, variableList));
}

domain_t NotExpression::getDomain(SearchNode &context, int time) const
{
    domain_t domain1 = mExpr1.getDomain(context, time);
    domain_t ret;
    for (int i : domain1) {
        if (i != 0) {
            ret.insert(0);
            break;
        }
    }
    if (domain1.find(0) != domain1.end()) {
        ret.insert(1);
    }
    return ret;
}

domain_t NotExpression::getInitialDomain() const
{
    domain_t domain1 = mExpr1.getInitialDomain();
    domain_t ret;
    for (int i : domain1) {
        if (i != 0) {
            ret.insert(0);
            break;
        }
    }
    if (domain1.find(0) != domain1.end()) {
        ret.insert(1);
    }
    return ret;
}
