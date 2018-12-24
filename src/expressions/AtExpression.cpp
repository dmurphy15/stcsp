#include "../../include/expressions/AtExpression.h"

#include <stdexcept>

#include "../../include/Variable.h"

#include "../../include/expressions/FirstExpression.h"
#include "../../include/expressions/specialExpressions/VariableExpression.h"
#include "../../include/expressions/specialExpressions/ConstantExpression.h"
#include "../../include/constraints/specialConstraints/PrimitiveAtConstraint.h"
#include "../../include/constraints/specialConstraints/PrimitiveNextConstraint.h"

AtExpression::AtExpression(Expression &a, ConstantExpression &b) :
        Expression({a, b}, false),
        mExpr1(a),
        mExpr2(b) {
    if (mExpr2.mConstant < 0) {
        throw std::logic_error("@ expression cannot be given a negative timepoint");
    }
}

int AtExpression::evaluate(SearchNode &context, int time) const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

Expression& AtExpression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList)
{
    // treat it as a first expression
    if (mExpr2.mConstant == 0) {
        Expression& f = *new FirstExpression(mExpr1);
        return f.normalize(constraintList, variableList);
    } else {
        Expression& equivalentExpr = mExpr1.normalize(constraintList, variableList);
        ConstantExpression& equivalentConstExpr =
                static_cast<ConstantExpression &>(mExpr2.normalize(constraintList, variableList));
        Variable& equivalentVar = *new Variable(equivalentExpr.getInitialDomain());
        variableList.insert(equivalentVar);
        VariableExpression& equivalentVarExpr = *new VariableExpression(equivalentVar);
        constraintList.insert(*new PrimitiveAtConstraint(equivalentVarExpr, equivalentExpr, equivalentConstExpr));
        constraintList.insert(*new PrimitiveNextConstraint(equivalentVarExpr, equivalentVarExpr));
        return equivalentVarExpr;
    }
}

Domain AtExpression::getDomain(SearchNode &context, int time) const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

Domain AtExpression::getInitialDomain() const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}
