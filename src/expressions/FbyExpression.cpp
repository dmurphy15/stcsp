#include "../../include/expressions/FbyExpression.h"

#include <stdexcept>

#include "../../include/Variable.h"

#include "../../include/expressions/specialExpressions/VariableExpression.h"
#include "../../include/expressions/specialExpressions/FirstExpression.h"
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

Expression& FbyExpression::normalize(std::set<Constraint_r> &constraintList,
                                     std::map<Expression_r, Expression_r> &normalizedMap,
                                     std::set<Variable_r> &variableList)
{
    auto it = normalizedMap.find(*this);
    if (it != normalizedMap.end()) {
        return it->second;
    }

    Expression &equivalentExpr1 = mExpr1.normalize(constraintList, normalizedMap, variableList);
    Expression &equivalentExpr2 = mExpr2.normalize(constraintList, normalizedMap, variableList);
    domain_t domain1 = equivalentExpr1.getInitialDomain();
    domain_t&& domain2 = equivalentExpr2.getInitialDomain();

    domain1.insert(domain2.begin(), domain2.end());
    Variable &var = *new Variable(domain1);
    variableList.insert(var);
    VariableExpression &varExpr = *new VariableExpression(var);

    normalizedMap.insert({*this, varExpr});
    normalizedMap.insert({varExpr, varExpr});

    Constraint& firsts = *new EqualConstraint(*new FirstExpression(varExpr), *new FirstExpression(equivalentExpr1));
    Constraint& nexts = *new EqualConstraint(*new NextExpression(varExpr), *new NextExpression(equivalentExpr2));
    firsts.normalize(constraintList, normalizedMap, variableList);
    nexts.normalize(constraintList, normalizedMap, variableList);

    return varExpr;
}

domain_t FbyExpression::getDomain(SearchNode &context, int time) const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

domain_t FbyExpression::getInitialDomain() const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}
