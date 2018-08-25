#include "../../include/expressions/LOrExpression.h"

#include "../../include/Variable.h"

LOrExpression::LOrExpression(Expression &a, Expression &b) :
        Expression({a, b}, true),
        mExpr1(a),
        mExpr2(b) {}

int LOrExpression::evaluate(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) || mExpr2.evaluate(context, time);
}

domain_t LOrExpression::getDomain(SearchNode &context, int time) const
{
    domain_t&& domain1 = mExpr1.getDomain(context, time);
    domain_t&& domain2 = mExpr2.getDomain(context, time);
    if (domain1.size() == 0 || domain2.size() == 0) {
        return {};
    }
    domain_t ret;
    if (domain1.find(0) != domain1.end() && domain2.find(0) != domain2.end()) {
        ret.insert(0);
    }
    for (int i : domain1) {
        if (i != 0) {
            ret.insert(1);
            return ret;
        }
    }
    for (int i : domain2) {
        if (i != 0) {
            ret.insert(1);
            return ret;
        }
    }
    return ret;
}

domain_t LOrExpression::getInitialDomain() const
{
    domain_t&& domain1 = mExpr1.getInitialDomain();
    domain_t&& domain2 = mExpr2.getInitialDomain();
    if (domain1.size() == 0 || domain2.size() == 0) {
        return {};
    }
    domain_t ret;
    if (domain1.find(0) != domain1.end() && domain2.find(0) != domain2.end()) {
        ret.insert(0);
    }
    for (int i : domain1) {
        if (i != 0) {
            ret.insert(1);
            return ret;
        }
    }
    for (int i : domain2) {
        if (i != 0) {
            ret.insert(1);
            return ret;
        }
    }
    return ret;
}
