#include "../../include/expressions/LAndExpression.h"

#include "../../include/Variable.h"

LAndExpression::LAndExpression(Expression &a, Expression &b) :
        Expression({a, b}, true),
        mExpr1(a),
        mExpr2(b) {}

int LAndExpression::evaluate(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) && mExpr2.evaluate(context, time);
}

Domain LAndExpression::getDomain(SearchNode &context, int time) const
{
    Domain&& domain1 = mExpr1.getDomain(context, time);
    Domain&& domain2 = mExpr2.getDomain(context, time);
    if (domain1.size() == 0 || domain2.size() == 0) {
        return {};
    }
    Domain ret;
    if (domain1.find(0) != domain1.end() || domain2.find(0) != domain2.end()) {
        ret.insert(0);
    }
    for (int i : domain1) {
        if (i != 0) {
            for (int j : domain2) {
                if (j != 0) {
                    ret.insert(1);
                    return ret;
                }
            }
        }
    }
    return ret;
}

Domain LAndExpression::getInitialDomain() const
{
    Domain&& domain1 = mExpr1.getInitialDomain();
    Domain&& domain2 = mExpr2.getInitialDomain();
    if (domain1.size() == 0 || domain2.size() == 0) {
        return {};
    }
    Domain ret;
    if (domain1.find(0) != domain1.end() || domain2.find(0) != domain2.end()) {
        ret.insert(0);
    }
    for (int i : domain1) {
        if (i != 0) {
            for (int j : domain2) {
                if (j != 0) {
                    ret.insert(1);
                    return ret;
                }
            }
        }
    }
    return ret;
}
