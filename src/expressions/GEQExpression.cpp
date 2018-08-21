#include "../../include/expressions/GEQExpression.h"

#include "../../include/Variable.h"

GEQExpression::GEQExpression(Expression &a, Expression &b) :
        mExpr1(a),
        mExpr2(b) {}

int GEQExpression::evaluate(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) >= mExpr2.evaluate(context, time);
}

std::set<Variable_r> GEQExpression::getVariables() const
{
    std::set<Variable_r> vars1 = mExpr1.getVariables();
    std::set<Variable_r> vars2 = mExpr2.getVariables();
    vars1.insert(vars2.begin(), vars2.end());
    return vars1;
}

Expression& GEQExpression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr1 = mExpr1.normalize(constraintList, variableList);
    Expression &equivalentExpr2 = mExpr2.normalize(constraintList, variableList);
    return *new GEQExpression(equivalentExpr1, equivalentExpr2);
}

domain_t GEQExpression::getDomain(SearchNode &context, int time) const
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

domain_t GEQExpression::getInitialDomain() const
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
