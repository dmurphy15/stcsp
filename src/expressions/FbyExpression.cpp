#include "../../include/expressions/FbyExpression.h"

#include <stdexcept>

#include "../../include/Variable.h"

#include "../../include/expressions/specialExpressions/VariableExpression.h"
#include "../../include/expressions/specialExpressions/ConstantExpression.h"
#include "../../include/expressions/AddExpression.h"
#include "../../include/expressions/MultiplyExpression.h"

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
    domain_t domain1 = equivalentExpr1.getInitialDomain();
    domain_t domain2 = equivalentExpr2.getInitialDomain();
    domain1.insert(domain2.begin(), domain2.end());
    Variable &var = *new Variable(domain1);
    variableList.insert(var);
    VariableExpression &varExp = *new VariableExpression(var);

    return *new AddExpression(equivalentExpr1, *new MultiplyExpression(*new ConstantExpression(-1), equivalentExpr2));
}

domain_t FbyExpression::getDomain(SearchNode &context, int time) const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

domain_t FbyExpression::getInitialDomain() const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}
