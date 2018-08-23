#include "../../include/constraints/GEConstraint.h"

#include <stdexcept>

#include "../../include/Variable.h"
#include "../../include/Expression.h"

#include "../../include/constraints/specialConstraints/EqualConstraint.h"
#include "../../include/expressions/specialExpressions/ConstantExpression.h"

#include "../../include/expressions/GEExpression.h"

#include "../../include/SearchNode.h"

GEConstraint::GEConstraint(Expression &a, Expression &b) :
        mExpr1(a),
        mExpr2(b) {}

GEConstraint::~GEConstraint() {}

void GEConstraint::normalize(std::set<Constraint_r> &constraintList,
                                std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr1 = mExpr1.normalize(constraintList, variableList);
    Expression &equivalentExpr2 = mExpr2.normalize(constraintList, variableList);
    Expression &GEQExpr = *new GEExpression(equivalentExpr1, equivalentExpr2);
    Constraint &equivalentConstraint = *new EqualConstraint(GEQExpr, *new ConstantExpression(1));
    constraintList.insert(equivalentConstraint);
}

int GEConstraint::isSatisfied(SearchNode &context, int time) const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

std::set<Variable_r> GEConstraint::getVariables() const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

std::vector<std::set<int>> GEConstraint::propagate(Variable &v, SearchNode &context)
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

bool GEConstraint::lt(const Constraint &rhs) const {
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

bool GEConstraint::eq(const Constraint &rhs) const {
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}