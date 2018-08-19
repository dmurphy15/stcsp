#include "../../include/expressions/NextExpression.h"

#include <stdexcept>

#include "../../include/Variable.h"
#include "../../include/specialConstraints/PrimitiveNextConstraint.h"
#include "../../include/specialConstraints/EqualConstraint.h"
#include "../../include/specialExpressions/VariableExpression.h"

NextExpression::NextExpression(Expression &a) : mExpr(a) {}

int NextExpression::evaluate(SearchNode &context, int time) const
{
    throw std::logic_error("Next expression should never have to be evaluated");
}

std::set<Variable_r> NextExpression::getVariables() const
{
    return mExpr.getVariables();
}

Expression& NextExpression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList)
{

    Expression &equivalentExpr = mExpr.normalize(constraintList, variableList);

    Variable &equivalentVar = *new Variable(equivalentExpr.getInitialDomain());
    // give it the same starting domain, since before propagation all future timepoints have the same domain as previous timepoints
    Variable &equivalentNextVar = *new Variable(equivalentExpr.getInitialDomain());
    VariableExpression &equivalentVarExpr = *new VariableExpression(equivalentVar);
    VariableExpression &equivalentNextVarExpr = *new VariableExpression(equivalentNextVar);

    constraintList.insert(*new EqualConstraint(equivalentNextVarExpr, equivalentExpr));
    constraintList.insert(*new PrimitiveNextConstraint(equivalentVarExpr, equivalentNextVarExpr));
    variableList.insert(equivalentVar);
    variableList.insert(equivalentNextVar);
    return equivalentVarExpr;
}

std::set<int> NextExpression::getDomain(SearchNode &context, int time) const
{
    throw std::logic_error("unimplemented");
}

std::set<int> NextExpression::getInitialDomain() const
{
    return mExpr.getInitialDomain();
}
