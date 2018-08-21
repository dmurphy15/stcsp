#include "../../include/constraints/GEQConstraint.h"

#include <stdexcept>

#include "../../include/Variable.h"
#include "../../include/Expression.h"

#include "../../include/specialConstraints/EqualConstraint.h"
#include "../../include/specialExpressions/ConstantExpression.h"

#include "../../include/expressions/GEQExpression.h"

#include "../../include/SearchNode.h"

GEQConstraint::GEQConstraint(Expression &a, Expression &b) :
        mExpr1(a),
        mExpr2(b) {}

GEQConstraint::~GEQConstraint() {}

void GEQConstraint::normalize(std::set<Constraint_r> &constraintList,
                                std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr1 = mExpr1.normalize(constraintList, variableList);
    Expression &equivalentExpr2 = mExpr2.normalize(constraintList, variableList);
    Expression &GEQExpr = *new GEQExpression(equivalentExpr1, equivalentExpr2);
    Constraint &equivalentConstraint = *new EqualConstraint(GEQExpr, *new ConstantExpression(1));
    constraintList.insert(equivalentConstraint);
}

int GEQConstraint::isSatisfied(SearchNode &context, int time) const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

std::set<Variable_r> GEQConstraint::getVariables() const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

std::vector<std::set<int>> GEQConstraint::propagate(Variable &v, SearchNode &context)
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

bool GEQConstraint::lt(const Constraint &rhs) const {
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

bool GEQConstraint::eq(const Constraint &rhs) const {
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}