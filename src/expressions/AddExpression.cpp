#include "../../include/expressions/AddExpression.h"

#include "../../include/Variable.h"

/**
 * the more similar the two subdomains are, the better we can take advantage of symmetry,
 * up to almost 2x speedup best case
 */
domain_t AddExpression::getDomain(SearchNode &context, int time) const
{
    domain_t&& tmp1 = mExpr1.getDomain(context, time);
    domain_t&& tmp2 = mExpr2.getDomain(context, time);
    std::set<int> shared;
    domain_t ret;
    for (int i : tmp1) {
        if (tmp2.find(i) == tmp2.end()) {
            for (int j : tmp2) {
                ret.insert(i + j);
            }
        } else {
            shared.insert(i);
        }
    }
    for (int i : tmp2) {
        if (shared.find(i) == shared.end()) {
            for (int j : tmp1) {
                ret.insert(i + j);
            }
        }
    }
    for (auto it = shared.begin(); it != shared.end(); it++) {
        for (auto it2 = it; it2 != shared.end(); it2++) {
            ret.insert(*it + *it2);
        }
    }
    return ret;
}

domain_t AddExpression::getInitialDomain() const
{
    domain_t&& tmp1 = mExpr1.getInitialDomain();
    domain_t&& tmp2 = mExpr2.getInitialDomain();
    std::set<int> shared;
    domain_t ret;
    for (int i : tmp1) {
        if (tmp2.find(i) == tmp2.end()) {
            for (int j : tmp2) {
                ret.insert(i + j);
            }
        } else {
            shared.insert(i);
        }
    }
    for (int i : tmp2) {
        if (shared.find(i) == shared.end()) {
            for (int j : tmp1) {
                ret.insert(i + j);
            }
        }
    }
    for (auto it = shared.begin(); it != shared.end(); it++) {
        for (auto it2 = it; it2 != shared.end(); it2++) {
            ret.insert(*it + *it2);
        }
    }
    return ret;
}
