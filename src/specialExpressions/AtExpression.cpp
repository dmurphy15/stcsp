#include "../../include/specialExpressions/AtExpression.h"

#include <stdexcept>

#include "../../include/Variable.h"

#include "../../include/expressions/NextExpression.h"
#include "../../include/specialExpressions/ConstantExpression.h"

AtExpression::AtExpression(Expression &a, ConstantExpression &b) :
        mExpr1(a),
        mExpr2(b) {}

int AtExpression::evaluate(SearchNode &context, int time) const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

std::set<Variable_r> AtExpression::getVariables() const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

Expression& AtExpression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList)
{
    Expression &currLayer = mExpr1;
    for (int i=0; i < mExpr2.mConstant; i++) {
        currLayer = *new NextExpression(currLayer);
    }
    return currLayer.normalize(constraintList, variableList);
}

domain_t AtExpression::getDomain(SearchNode &context, int time) const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

domain_t AtExpression::getInitialDomain() const
{
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}

bool AtExpression::lt(const Expression &rhs) const {
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}
bool AtExpression::eq(const Expression &rhs) const {
    throw std::logic_error(std::string(__FILE__) + "has been implemented as a stub; normalization should have removed it");
}
