#include "VariableExpression.h"

#include "../Variable.h"
#include "../Constraint.h"

VariableExpression::VariableExpression(Variable &v) :
        mVariable(v) {}

int VariableExpression::evaluate(InstantSolver &context) const
{
    return mVariable.evaluate(context);
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

std::set<int> VariableExpression::getDomain(InstantSolver &context) const {
    return mVariable.getDomain(context);
}

std::set<int> VariableExpression::getInitialDomain() const {
    return mVariable.getInitialDomain();
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