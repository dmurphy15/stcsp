#include "../../include/expressions/IfThenElseExpression.h"

#include "../../include/Variable.h"

IfThenElseExpression::IfThenElseExpression(Expression &ifExpr, Expression &thenExpr, Expression &elseExpr) :
        Expression({ifExpr, thenExpr, elseExpr}, false),
        mExpr1(ifExpr),
        mExpr2(thenExpr),
        mExpr3(elseExpr) {}

int IfThenElseExpression::evaluate(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) != 0 ? mExpr2.evaluate(context, time) : mExpr3.evaluate(context, time);
}

Domain IfThenElseExpression::getDomain(SearchNode &context, int time) const
{
    Domain&& domain1 = mExpr1.getDomain(context, time);
    Domain ret;
    for (int i : domain1) {
        if (i != 0) {
            ret = mExpr2.getDomain(context, time);
            break;
        }
    }
    if (domain1.find(0) != domain1.end()) {
        domain_t&& domain3 = mExpr3.getDomain(context, time);
        ret.insert(domain3.begin(), domain3.end());
    }
    return ret;
}

Domain IfThenElseExpression::getInitialDomain() const
{
    Domain&& domain1 = mExpr1.getInitialDomain();
    Domain ret;
    for (int i : domain1) {
        if (i != 0) {
            ret = mExpr2.getInitialDomain();
            break;
        }
    }
    if (domain1.find(0) != domain1.end()) {
        domain_t&& domain3 = mExpr3.getInitialDomain();
        ret.insert(domain3.begin(), domain3.end());
    }
    return ret;
}
