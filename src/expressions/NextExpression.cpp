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

Expression& NextExpression::normalize(std::set<Constraint_r> &constraintList,
                                      std::map<Expression_r, Expression_r> &normalizedMap,
                                      std::set<Variable_r> &variableList)
{
    auto it = normalizedMap.find(*this);
    if (it != normalizedMap.end()) {
        return it->second;
    }

    Expression &equivalentExpr = mExpr.normalize(constraintList, normalizedMap, variableList);
    domain_t&& d = equivalentExpr.getInitialDomain();
    Variable &equivalentVar = *new Variable(d);
    VariableExpression &equivalentVarExpr = *new VariableExpression(equivalentVar);

    if (typeid(mExpr) == typeid(VariableExpression)) { // if our expression is next(x), we can take this shortcut
        constraintList.insert(*new PrimitiveNextConstraint(equivalentVarExpr, static_cast<VariableExpression &>(mExpr)));
    } else {
        // give it the same starting domain, since before propagation all future timepoints have the same domain as previous timepoints
        Variable &equivalentNextVar = *new Variable(d);
        VariableExpression &equivalentNextVarExpr = *new VariableExpression(equivalentNextVar);
        constraintList.insert(*new EqualConstraint(equivalentNextVarExpr, equivalentExpr));
        constraintList.insert(*new PrimitiveNextConstraint(equivalentVarExpr, equivalentNextVarExpr));
        variableList.insert(equivalentNextVar);
    }

    variableList.insert(equivalentVar);

    normalizedMap.insert({*this, equivalentVarExpr});
    normalizedMap.insert({equivalentVarExpr, equivalentVarExpr});
    return equivalentVarExpr;
}

domain_t NextExpression::getDomain(SearchNode &context, int time) const
{
    throw std::logic_error("should have been removed by normalization");
}

domain_t NextExpression::getInitialDomain() const
{
    throw std::logic_error("should have been removed by normalization");
}
