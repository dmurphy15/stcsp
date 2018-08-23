#include "../../../include/expressions/specialExpressions/VariableExpression.h"

#include "../../../include/Variable.h"
#include "../../../include/Constraint.h"

VariableExpression::VariableExpression(Variable &v) :
        Expression({}, false),
        mVariable(v) {}

int VariableExpression::evaluate(SearchNode &context, int time) const
{
    return mVariable.evaluate(context, time);
}

std::set<Variable_r> VariableExpression::getVariables() const
{
    return {mVariable};
}

Expression& VariableExpression::normalize(std::set<Constraint_r> &constraintList,
                                          std::set<Variable_r> &variableList)
{
    return *this;
}

domain_t VariableExpression::getDomain(SearchNode &context, int time) const {
    return mVariable.getDomain(context, time);
}

domain_t VariableExpression::getInitialDomain() const {
    // doing this so that I can get a mutable copy of the const vector that the variable returns
    domain_t a = mVariable.getInitialDomain();
    return a;
}

bool VariableExpression::lt(const Expression &rhs) const {
    return (typeid(*this).before(typeid(rhs))) ||
            ((typeid(*this) == typeid(rhs)) &&
                (mVariable < static_cast<const VariableExpression&>(rhs).mVariable));
}
bool VariableExpression::eq(const Expression &rhs) const {
    return (typeid(*this) == typeid(rhs)) &&
        (mVariable == static_cast<const VariableExpression&>(rhs).mVariable);
}