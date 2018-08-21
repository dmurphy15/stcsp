#include "../../include/expressions/SubtractExpression.h"

#include <stdexcept>

#include "../../include/Variable.h"

#include "../../include/expressions/AddExpression.h"
#include "../../include/expressions/MultiplyExpression.h"
#include "../../include/specialExpressions/ConstantExpression.h"

SubtractExpression::SubtractExpression(Expression &a, Expression &b) :
        mExpr1(a),
        mExpr2(b) {}

int SubtractExpression::evaluate(SearchNode &context, int time) const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

std::set<Variable_r> SubtractExpression::getVariables() const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

Expression& SubtractExpression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr1 = mExpr1.normalize(constraintList, variableList);
    Expression &equivalentExpr2 = mExpr2.normalize(constraintList, variableList);
    return *new AddExpression(equivalentExpr1, *new MultiplyExpression(*new ConstantExpression(-1), equivalentExpr2));
}

domain_t SubtractExpression::getDomain(SearchNode &context, int time) const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

domain_t SubtractExpression::getInitialDomain() const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

bool SubtractExpression::lt(const Expression &rhs) const {
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}
bool SubtractExpression::eq(const Expression &rhs) const {
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}
