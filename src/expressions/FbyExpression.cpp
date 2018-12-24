#include "../../include/expressions/FbyExpression.h"

#include <stdexcept>

#include "../../include/Variable.h"

#include "../../include/expressions/specialExpressions/VariableExpression.h"
#include "../../include/expressions/FirstExpression.h"
#include "../../include/expressions/NextExpression.h"
#include "../../include/constraints/specialConstraints/EqualConstraint.h"

FbyExpression::FbyExpression(Expression &a, Expression &b) :
        Expression({a, b}, false),
        mExpr1(a),
        mExpr2(b) {}

int FbyExpression::evaluate(SearchNode &context, int time) const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

Expression& FbyExpression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr1 = mExpr1.normalize(constraintList, variableList);
    Expression &equivalentExpr2 = mExpr2.normalize(constraintList, variableList);
    Domain domain1 = equivalentExpr1.getInitialDomain();
    Domain&& domain2 = equivalentExpr2.getInitialDomain();

    domain1.insert(domain2);
    Variable &var = *new Variable(domain1);
    variableList.insert(var);
    VariableExpression &varExpr = *new VariableExpression(var);

    Constraint& firsts = *new EqualConstraint(*new FirstExpression(varExpr), *new FirstExpression(equivalentExpr1));
    Constraint& nexts = *new EqualConstraint(*new NextExpression(varExpr), *new NextExpression(equivalentExpr2));
    firsts.normalize(constraintList, variableList);
    nexts.normalize(constraintList, variableList);

    return varExpr;
}

Domain FbyExpression::getDomain(SearchNode &context, int time) const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

Domain FbyExpression::getInitialDomain() const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}
