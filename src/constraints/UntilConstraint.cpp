#include "../../include/constraints/UntilConstraint.h"

#include "../../include/Variable.h"
#include "../../include/Expression.h"
#include "../../include/expressions/specialExpressions/VariableExpression.h"
#include "../../include/constraints/specialConstraints/PrimitiveUntilConstraint.h"
#include "../../include/constraints/specialConstraints/EqualConstraint.h"

#include "../../include/SearchNode.h"

UntilConstraint::UntilConstraint(Expression &a, Expression &b, int expressionSetId) :
        Constraint({a, b}, false, expressionSetId),
        mExpr(a),
        mUntilExpr(b) {}

UntilConstraint::~UntilConstraint() {}

// I don't think we actually need to normalize this, since its all contained within 1 timepoint
// then again, the auxiliary variables could speed up consistency enforcement
void UntilConstraint::normalize(std::set<Constraint_r> &constraintList,
                                std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr1 = mExpr.normalize(constraintList, variableList);
    Expression &equivalentExpr2 = mUntilExpr.normalize(constraintList, variableList);
    Variable &var1 = *new Variable(equivalentExpr1.getInitialDomain());
    Variable &var2 = *new Variable(equivalentExpr2.getInitialDomain());
    VariableExpression &varExpr1 = *new VariableExpression(var1);
    VariableExpression &varExpr2 = *new VariableExpression(var2);
    constraintList.insert(*new EqualConstraint(varExpr1, equivalentExpr1));
    constraintList.insert(*new EqualConstraint(varExpr2, equivalentExpr2));
    Constraint &equivalentConstraint = *new PrimitiveUntilConstraint(varExpr1, varExpr2);

    variableList.insert(var1);
    variableList.insert(var2);
    constraintList.insert(equivalentConstraint);
}

bool UntilConstraint::isSatisfied(SearchNode &context, int time) const
{
    return (mExpr.evaluate(context, time) != 0) || (mUntilExpr.evaluate(context, time) != 0);
}

std::vector<std::set<int>> UntilConstraint::propagate(Variable &v, SearchNode &context)
{
    return context.defaultPropagate(v, *this);
}