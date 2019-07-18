#include "../../include/expressions/AtExpression.h"

#include <stdexcept>

#include "../../include/Variable.h"

#include "../../include/expressions/FirstExpression.h"
#include "../../include/expressions/specialExpressions/VariableExpression.h"
#include "../../include/expressions/specialExpressions/ConstantExpression.h"
#include "../../include/constraints/specialConstraints/PrimitiveAtConstraint.h"
#include "../../include/constraints/specialConstraints/PrimitiveNextConstraint.h"
#include "../../include/constraints/specialConstraints/EqualConstraint.h"

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

//        return equivalentVarExpr;
        // an AtExpression returns something whose value is constant anyway, so we may as well return a
        // FirstExpression, so that we can detect tautologies from the start (though we still want
        // to keep the added PrimitiveNextConstraint, so we dont have to propagate over the entire
        // domain of the added variable at each timepoint)
//        return (*new FirstExpression(equivalentVarExpr)).normalize(constraintList, variableList);
        return (equivalentVarExpr).normalize(constraintList, variableList);
    }
}

domain_t AtExpression::getDomain(SearchNode &context, int time) const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

domain_t AtExpression::getInitialDomain() const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}
