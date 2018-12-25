#include "../../../include/expressions/specialExpressions/ConstantExpression.h"
#include "../../../include/Domain.h"


ConstantExpression::ConstantExpression(int constant) :
        Expression({}, false),
        mConstant(constant) {}

int ConstantExpression::evaluate(SearchNode &context, int time) const
{
    return mConstant;
}

void ConstantExpression::getVariables(std::set<Variable_r>& variables) const
{
    return;
}

Domain ConstantExpression::getDomain(SearchNode &context, int time) const
{
    return Domain({mConstant});
}

Domain ConstantExpression::getInitialDomain() const
{
    return Domain({mConstant});
}

