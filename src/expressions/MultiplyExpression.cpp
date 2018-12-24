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

Domain MultiplyExpression::getDomain(SearchNode &context, int time) const
{
    Domain&& tmp1 = mExpr1.getDomain(context, time);
    Domain&& tmp2 = mExpr2.getDomain(context, time);
    std::set<int> shared;
    Domain ret;
    for (int i : tmp1) {
        if (tmp2.find(i) == tmp2.end()) {
            for (int j : tmp2) {
                ret.insert(i * j);
            }
        } else {
            shared.insert(i);
        }
    }
    for (int i : tmp2) {
        if (shared.find(i) == shared.end()) {
            for (int j : tmp1) {
                ret.insert(i * j);
            }
        }
    }
    for (auto it = shared.begin(); it != shared.end(); it++) {
        for (auto it2 = it; it2 != shared.end(); it2++) {
            ret.insert(*it * *it2);
        }
    }
    return ret;
}

Domain MultiplyExpression::getInitialDomain() const
{
    Domain&& tmp1 = mExpr1.getInitialDomain();
    Domain&& tmp2 = mExpr2.getInitialDomain();
    std::set<int> shared;
    Domain ret;
    for (int i : tmp1) {
        if (tmp2.find(i) == tmp2.end()) {
            for (int j : tmp2) {
                ret.insert(i * j);
            }
        } else {
            shared.insert(i);
        }
    }
    for (int i : tmp2) {
        if (shared.find(i) == shared.end()) {
            for (int j : tmp1) {
                ret.insert(i * j);
            }
        }
    }
    for (auto it = shared.begin(); it != shared.end(); it++) {
        for (auto it2 = it; it2 != shared.end(); it2++) {
            ret.insert(*it * *it2);
        }
    }
    return ret;
}
