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

// create a new auxiliary variable, set it equal to the expression for the first timepoint (primitivefirstconstraint
// will be removed after the first timepoint) and return the auxiliary variable (so that we can later swap out the
// expression and use a fixed value)
Expression& FirstExpression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList)
{

//    HEY!!!!!!YOU HAVE TO FIX THE NORMALIZATION TECHNIQUES TO CORRESPOND TO THOSE IN THE PAPER...
//    ALSO YOU SHOULD USE DECLTYPE TO CREATE GENERIC COMPARISON OPERATORS. EVERYTHING SHOULD HAVE A MEXPRESSIONS CONTAINER, WHICH IS A SORTED SET FOR SYMMETRIC EXPRESSIONS AND A VECTOR FOR NON-SYMMETRIC ONES, AND THE GENERIC COMPARATORS CAN JUST COMPARE THE EQUALITY OF MEXPRESSION



    Expression &equivalentExpr = mExpr.normalize(constraintList, variableList);

    Variable &equivalentVar = *new Variable(equivalentExpr.getInitialDomain());
    VariableExpression &equivalentVarExpr = *new VariableExpression(equivalentVar);

    constraintList.insert(*new PrimitiveFirstConstraint(equivalentVarExpr, equivalentExpr));
    constraintList.insert(*new PrimitiveNextConstraint(equivalentVarExpr, equivalentVarExpr));
    variableList.insert(equivalentVar);
    return equivalentVarExpr;
}

domain_t FirstExpression::getDomain(SearchNode &context, int time) const
{
    throw std::logic_error("these should be removed with normalization");
}

domain_t FirstExpression::getInitialDomain() const
{
    throw std::logic_error("these should be removed with normalization");
}
