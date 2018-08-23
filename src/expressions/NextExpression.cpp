#include "../../include/expressions/NextExpression.h"

#include <stdexcept>

#include "../../include/Variable.h"
#include "../../include/constraints/specialConstraints/PrimitiveNextConstraint.h"
#include "../../include/constraints/specialConstraints/EqualConstraint.h"
#include "../../include/expressions/specialExpressions/VariableExpression.h"

NextExpression::NextExpression(Expression &a) : Expression({a}, false), mExpr(a) {}

int NextExpression::evaluate(SearchNode &context, int time) const
{
    throw std::logic_error("Next expression should never have to be evaluated");
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

domain_t NextExpression::getDomain(SearchNode &context, int time) const
{
    throw std::logic_error("unimplemented");
}

domain_t NextExpression::getInitialDomain() const
{
    return mExpr.getInitialDomain();
}
