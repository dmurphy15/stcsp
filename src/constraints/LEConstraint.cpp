#include "../../include/constraints/LEConstraint.h"

#include <stdexcept>

#include "../../include/Variable.h"
#include "../../include/Expression.h"

#include "../../include/constraints/specialConstraints/EqualConstraint.h"
#include "../../include/expressions/specialExpressions/ConstantExpression.h"

#include "../../include/expressions/LEExpression.h"

#include "../../include/SearchNode.h"

LEConstraint::LEConstraint(Expression &a, Expression &b) :
        mExpr1(a),
        mExpr2(b) {}

LEConstraint::~LEConstraint() {}

void LEConstraint::normalize(std::set<Constraint_r> &constraintList,
                                std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr1 = mExpr1.normalize(constraintList, variableList);
    Expression &equivalentExpr2 = mExpr2.normalize(constraintList, variableList);
    Expression &LEQExpr = *new LEExpression(equivalentExpr1, equivalentExpr2);
    Constraint &equivalentConstraint = *new EqualConstraint(LEQExpr, *new ConstantExpression(1));
    constraintList.insert(equivalentConstraint);
}

int LEConstraint::isSatisfied(SearchNode &context, int time) const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

std::set<Variable_r> LEConstraint::getVariables() const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

std::vector<std::set<int>> LEConstraint::propagate(Variable &v, SearchNode &context)
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

bool LEConstraint::lt(const Constraint &rhs) const {
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

bool LEConstraint::eq(const Constraint &rhs) const {
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}