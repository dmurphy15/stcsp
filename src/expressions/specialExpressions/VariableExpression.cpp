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

void VariableExpression::getVariables(std::set<Variable_r>& variables) const
{
    variables.insert(mVariable);
}

Domain VariableExpression::getDomain(SearchNode &context, int time) const {
    return mVariable.getDomain(context, time);
}

Domain VariableExpression::getInitialDomain() const {
    // doing this so that I can get a mutable copy of the const vector that the variable returns
    Domain a = mVariable.getInitialDomain();
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