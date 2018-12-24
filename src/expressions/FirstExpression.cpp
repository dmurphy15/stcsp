#include "../../include/expressions/FirstExpression.h"

#include <stdexcept>

#include "../../include/Variable.h"
#include "../../include/constraints/specialConstraints/PrimitiveFirstConstraint.h"
#include "../../include/constraints/specialConstraints/PrimitiveNextConstraint.h"
#include "../../include/expressions/specialExpressions/VariableExpression.h"

FirstExpression::FirstExpression(Expression &a) : Expression({a}, false), mExpr(a) {}

int FirstExpression::evaluate(SearchNode &context, int time) const
{
    throw std::logic_error("these should be removed with normalization");
}

// this only creates 1 auxiliary variable instead of 2. I believe this should be faster, because the 2nd
// auxiliary variable would have been obsoleted after the first time point, and we would have been stuck making
// its value consistent with the value of the expression for all future timepoints
Expression& FirstExpression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr = mExpr.normalize(constraintList, variableList);

    Variable &equivalentVar = *new Variable(equivalentExpr.getInitialDomain());
    VariableExpression &equivalentVarExpr = *new VariableExpression(equivalentVar);

    constraintList.insert(*new PrimitiveFirstConstraint(equivalentVarExpr, equivalentExpr));
    constraintList.insert(*new PrimitiveNextConstraint(equivalentVarExpr, equivalentVarExpr));
    variableList.insert(equivalentVar);
    return equivalentVarExpr;
}

Domain FirstExpression::getDomain(SearchNode &context, int time) const
{
    throw std::logic_error("these should be removed with normalization");
}

Domain FirstExpression::getInitialDomain() const
{
    throw std::logic_error("these should be removed with normalization");
}
