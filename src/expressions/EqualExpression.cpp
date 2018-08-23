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

Expression& EqualExpression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr1 = mExpr1.normalize(constraintList, variableList);
    Expression &equivalentExpr2 = mExpr2.normalize(constraintList, variableList);
    return *new EqualExpression(equivalentExpr1, equivalentExpr2);
}

domain_t EqualExpression::getDomain(SearchNode &context, int time) const
{
    domain_t domain1 = mExpr1.getDomain(context, time);
    domain_t domain2 = mExpr2.getDomain(context, time);

    if (domain1.size() == 0 || domain2.size() == 0) {
        return {};
    }
//    else if (domain1.size() == 0 || domain2.size() == 0) {
//        return {0};
//    }

    // now we know the domains are not empty

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
    domain_t domain1 = mExpr1.getInitialDomain();
    domain_t domain2 = mExpr2.getInitialDomain();

    if (domain1.size() == 0 || domain2.size() == 0) {
        return {};
    }
//    else if (domain1.size() == 0 || domain2.size() == 0) {
//        return {0};
//    }

    // now we know the domains are not empty

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
