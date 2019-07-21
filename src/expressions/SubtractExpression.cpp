#include "../../include/expressions/SubtractExpression.h"

#include <stdexcept>

#include "../../include/Variable.h"

#include "../../include/expressions/AddExpression.h"
#include "../../include/expressions/MultiplyExpression.h"
#include "../../include/expressions/specialExpressions/ConstantExpression.h"

SubtractExpression::SubtractExpression(Expression &a, Expression &b) :
        Expression({a, b}, false),
        mExpr1(a),
        mExpr2(b) {}

int SubtractExpression::evaluate(SearchNode &context, int time) const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

Expression& SubtractExpression::normalize(std::set<Constraint_r> &constraintList,
                                          std::map<Expression_r, Expression_r> &normalizedMap,
                                          std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr1 = mExpr1.normalize(constraintList, normalizedMap, variableList);
    Expression &equivalentExpr2 = mExpr2.normalize(constraintList, normalizedMap, variableList);
    Expression& normalized = *new AddExpression(equivalentExpr1, *new MultiplyExpression(*new ConstantExpression(-1), equivalentExpr2));
    normalizedMap.insert({*this, normalized});
    normalizedMap.insert({normalized, normalized});
    return normalized;
}

domain_t SubtractExpression::getDomain(SearchNode &context, int time) const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

domain_t SubtractExpression::getInitialDomain() const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}
