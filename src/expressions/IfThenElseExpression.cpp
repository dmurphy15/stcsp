#include "../../include/expressions/IfThenElseExpression.h"

#include "../../include/Variable.h"

IfThenElseExpression::IfThenElseExpression(Expression &ifExpr, Expression &thenExpr, Expression &elseExpr) :
        mExpr1(ifExpr),
        mExpr2(thenExpr),
        mExpr3(elseExpr){}

int IfThenElseExpression::evaluate(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) >= mExpr2.evaluate(context, time);
}

std::set<Variable_r> IfThenElseExpression::getVariables() const
{
    std::set<Variable_r> vars1 = mExpr1.getVariables();
    std::set<Variable_r> vars2 = mExpr2.getVariables();
    std::set<Variable_r> vars3 = mExpr3.getVariables();
    vars1.insert(vars2.begin(), vars2.end());
    vars1.insert(vars3.begin(), vars3.end());
    return vars1;
}

Expression& IfThenElseExpression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr1 = mExpr1.normalize(constraintList, variableList);
    Expression &equivalentExpr2 = mExpr2.normalize(constraintList, variableList);
    Expression &equivalentExpr3 = mExpr3.normalize(constraintList, variableList);
    return *new IfThenElseExpression(equivalentExpr1, equivalentExpr2, equivalentExpr3);
}

domain_t IfThenElseExpression::getDomain(SearchNode &context, int time) const
{
    domain_t domain1 = mExpr1.getDomain(context, time);
    domain_t ret;
    for (int i : domain1) {
        if (i != 0) {
            ret = mExpr2.getDomain(context, time);
            break;
        }
    }
    if (domain1.find(0) != domain1.end()) {
        domain_t domain3 = mExpr3.getDomain(context, time);
        ret.insert(domain3.begin(), domain3.end());
    }
    return ret;
}

domain_t IfThenElseExpression::getInitialDomain() const
{
    domain_t domain1 = mExpr1.getInitialDomain();
    domain_t ret;
    for (int i : domain1) {
        if (i != 0) {
            ret = mExpr2.getInitialDomain();
            break;
        }
    }
    if (domain1.find(0) != domain1.end()) {
        domain_t domain3 = mExpr3.getInitialDomain();
        ret.insert(domain3.begin(), domain3.end());
    }
    return ret;
}
