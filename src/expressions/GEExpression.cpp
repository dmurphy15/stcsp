#include "../../include/expressions/GEExpression.h"

#include "../../include/Variable.h"

GEExpression::GEExpression(Expression &a, Expression &b) :
        Expression({a, b}, false),
        mExpr1(a),
        mExpr2(b) {}

int GEExpression::evaluate(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) >= mExpr2.evaluate(context, time);
}

Domain GEExpression::getDomain(SearchNode &context, int time) const
{
    Domain&& domain1 = mExpr1.getDomain(context, time);
    Domain&& domain2 = mExpr2.getDomain(context, time);
    if (domain1.size() == 0 || domain2.size() == 0) {
        return {};
    }
    bool found0, found1 = false;
    Domain ret;
    for (int i : domain1) {
        for (int j : domain2) {
            if (i >= j && !found1) {
                ret.insert(1);
                found1 = true;
            } else if (!found0) {
                ret.insert(0);
                found0 = true;
            }
            if (found1 && found0) {
                break;
            }
        }
    }
    return ret;
}

Domain GEExpression::getInitialDomain() const
{
    Domain&& domain1 = mExpr1.getInitialDomain();
    Domain&& domain2 = mExpr2.getInitialDomain();
    if (domain1.size() == 0 || domain2.size() == 0) {
        return {};
    }
    bool found0, found1 = false;
    Domain ret;
    for (int i : domain1) {
        for (int j : domain2) {
            if (i >= j && !found1) {
                ret.insert(1);
                found1 = true;
            } else if (!found0) {
                ret.insert(0);
                found0 = true;
            }
            if (found1 && found0) {
                break;
            }
        }
    }
    return ret;
}
