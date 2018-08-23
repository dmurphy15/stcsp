#include "../../include/expressions/LEQExpression.h"

#include "../../include/Variable.h"

LEQExpression::LEQExpression(Expression &a, Expression &b) :
        Expression({a, b}, false),
        mExpr1(a),
        mExpr2(b) {}

int LEQExpression::evaluate(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) <= mExpr2.evaluate(context, time);
}

Expression& LEQExpression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr1 = mExpr1.normalize(constraintList, variableList);
    Expression &equivalentExpr2 = mExpr2.normalize(constraintList, variableList);
    return *new LEQExpression(equivalentExpr1, equivalentExpr2);
}

domain_t LEQExpression::getDomain(SearchNode &context, int time) const
{
    domain_t domain1 = mExpr1.getDomain(context, time);
    domain_t domain2 = mExpr2.getDomain(context, time);

    if (domain1.size() == 0 || domain2.size() == 0) {
        return {};
    }

    // now we know the domains are not empty
    bool found0, found1 = false;
    domain_t ret;
    for (int i : domain1) {
        for (int j : domain2) {
            if (i <= j && !found1) {
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

domain_t LEQExpression::getInitialDomain() const
{
    domain_t domain1 = mExpr1.getInitialDomain();
    domain_t domain2 = mExpr2.getInitialDomain();

    if (domain1.size() == 0 || domain2.size() == 0) {
        return {};
    }
    bool found0, found1 = false;
    domain_t ret;
    for (int i : domain1) {
        for (int j : domain2) {
            if (i <= j && !found1) {
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
