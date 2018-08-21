#include "../../include/constraints/LEQConstraint.h"

#include <stdexcept>

#include "../../include/Variable.h"
#include "../../include/Expression.h"

#include "../../include/specialConstraints/EqualConstraint.h"
#include "../../include/specialExpressions/ConstantExpression.h"

#include "../../include/expressions/LEQExpression.h"

#include "../../include/SearchNode.h"

LEQConstraint::LEQConstraint(Expression &a, Expression &b) :
        mExpr1(a),
        mExpr2(b) {}

LEQConstraint::~LEQConstraint() {}

void LEQConstraint::normalize(std::set<Constraint_r> &constraintList,
                                std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr1 = mExpr1.normalize(constraintList, variableList);
    Expression &equivalentExpr2 = mExpr2.normalize(constraintList, variableList);
    Expression &LEQExpr = *new LEQExpression(equivalentExpr1, equivalentExpr2);
    Constraint &equivalentConstraint = *new EqualConstraint(LEQExpr, *new ConstantExpression(1));
    constraintList.insert(equivalentConstraint);
}

int LEQConstraint::isSatisfied(SearchNode &context, int time) const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

std::set<Variable_r> LEQConstraint::getVariables() const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

std::vector<std::set<int>> LEQConstraint::propagate(Variable &v, SearchNode &context)
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

bool LEQConstraint::lt(const Constraint &rhs) const {
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

bool LEQConstraint::eq(const Constraint &rhs) const {
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}