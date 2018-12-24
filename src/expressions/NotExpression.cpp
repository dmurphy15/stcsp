#include "../../include/expressions/NotExpression.h"

#include "../../include/Variable.h"

NotExpression::NotExpression(Expression &a) :
        Expression({a}, false),
        mExpr1(a) {}

int NotExpression::evaluate(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) != 0 ? 0 : 1;
}

Domain NotExpression::getDomain(SearchNode &context, int time) const
{
    Domain domain1 = mExpr1.getDomain(context, time);
    Domain ret;
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

Domain NotExpression::getInitialDomain() const
{
    Domain domain1 = mExpr1.getInitialDomain();
    Domain ret;
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
