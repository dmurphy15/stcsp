#include "../../include/expressions/EqualExpression.h"

#include "../../include/Variable.h"

EqualExpression::EqualExpression(Expression &a, Expression &b) :
        Expression({a, b}, true),
        mExpr1(a),
        mExpr2(b) {}

int EqualExpression::evaluate(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) == mExpr2.evaluate(context, time);
}

domain_t EqualExpression::getDomain(SearchNode &context, int time) const
{
    domain_t&& domain1 = mExpr1.getDomain(context, time);
    domain_t&& domain2 = mExpr2.getDomain(context, time);
    if (domain1.size() == 0 || domain2.size() == 0) {
        return {};
    }
    domain_t ret;
    for (int i : domain1) {
        if (domain2.find(i) != domain2.end()) {
            ret.insert(1);
            break;
        }
    }
    if (domain1.size() != 1 || domain2.size() != 1 || *domain1.begin() != *domain2.begin()) {
        ret.insert(0);
    }
    return ret;
}

domain_t EqualExpression::getInitialDomain() const
{
    domain_t&& domain1 = mExpr1.getInitialDomain();
    domain_t&& domain2 = mExpr2.getInitialDomain();
    if (domain1.size() == 0 || domain2.size() == 0) {
        return {};
    }
    domain_t ret;
    for (int i : domain1) {
        if (domain2.find(i) != domain2.end()) {
            ret.insert(1);
            break;
        }
    }
    if (domain1.size() != 1 || domain2.size() != 1 || *domain1.begin() != *domain2.begin()) {
        ret.insert(0);
    }
    return ret;
}
