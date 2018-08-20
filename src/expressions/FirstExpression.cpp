#include "../../include/expressions/FirstExpression.h"

#include <stdexcept>

#include "../../include/Variable.h"
#include "../../include/specialConstraints/PrimitiveFirstConstraint.h"
#include "../../include/specialExpressions/VariableExpression.h"

FirstExpression::FirstExpression(Expression &a) : mExpr(a) {}

int FirstExpression::evaluate(SearchNode &context, int time) const
{
    throw std::logic_error("these should be removed with normalization");
//    return mExpr.evaluate(context, 0);
}

std::set<Variable_r> FirstExpression::getVariables() const
{
    return mExpr.getVariables();
}

// create a new auxiliary variable, set it equal to the expression for the first timepoint (primitivefirstconstraint
// will be removed after the first timepoint) and return the auxiliary variable (so that we can later swap out the
// expression and use a fixed value)
Expression& FirstExpression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList)
{

    Expression &equivalentExpr = mExpr.normalize(constraintList, variableList);

    Variable &equivalentVar = *new Variable(equivalentExpr.getInitialDomain());
    VariableExpression &equivalentVarExpr = *new VariableExpression(equivalentVar);

    constraintList.insert(*new PrimitiveFirstConstraint(equivalentVarExpr, equivalentExpr));
    variableList.insert(equivalentVar);
    return equivalentVarExpr;
}

domain_t FirstExpression::getDomain(SearchNode &context, int time) const
{
    return mExpr.getDomain(context, 0);
}

domain_t FirstExpression::getInitialDomain() const
{
    return mExpr.getInitialDomain();
}
