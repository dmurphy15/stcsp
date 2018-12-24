#include "../../include/expressions/NEQExpression.h"

#include "../../include/Variable.h"

NEQExpression::NEQExpression(Expression &a, Expression &b) :
        Expression({a, b}, true),
        mExpr1(a),
        mExpr2(b) {}

int NEQExpression::evaluate(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) != mExpr2.evaluate(context, time);
}

Domain NEQExpression::getDomain(SearchNode &context, int time) const
{
    Domain&& domain1 = mExpr1.getDomain(context, time);
    Domain&& domain2 = mExpr2.getDomain(context, time);
    if (domain1.size() == 0 || domain2.size() == 0) {
        return {};
    }
    Domain ret;
    for (int i : domain1) {
        if (domain2.find(i) != domain2.end()) {
            ret.insert(0);
            break;
        }
    }
    if (domain1.size() != 1 || domain2.size() != 1 || *domain1.begin() != *domain2.begin()) {
        ret.insert(1);
    }
    return ret;
}

Domain NEQExpression::getInitialDomain() const
{
    Domain&& domain1 = mExpr1.getInitialDomain();
    Domain&& domain2 = mExpr2.getInitialDomain();
    if (domain1.size() == 0 || domain2.size() == 0) {
        return {};
    }
    Domain ret;
    for (int i : domain1) {
        if (domain2.find(i) != domain2.end()) {
            ret.insert(0);
            break;
        }
    }
    if (domain1.size() != 1 || domain2.size() != 1 || *domain1.begin() != *domain2.begin()) {
        ret.insert(1);
    }
    return ret;
}
