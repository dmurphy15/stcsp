#include "PrimitiveNextConstraint.h"

#include "../expressions/VariableExpression.h"
#include "../Variable.h"

PrimitiveNextConstraint::PrimitiveNextConstraint(VariableExpression &variable, VariableExpression &nextVariable) :
        Constraint({variable, nextVariable}),
        mVariable(*variable.getVariables().begin()),
        mNextVariable(*nextVariable.getVariables().begin()) {}

PrimitiveNextConstraint::~PrimitiveNextConstraint() {}

void PrimitiveNextConstraint::normalize(std::set<Constraint_r> &constraintList,
                                std::set<Variable_r> &variableList)
{
    // can do this since normalizing the member variableExpressions would do nothing anyway
    constraintList.insert(*this);
}

int PrimitiveNextConstraint::isSatisfied(InstantSolver &context) const
{
    return true;
}

std::set<Variable_r> PrimitiveNextConstraint::getVariables() const
{
    return {mVariable, mNextVariable};
}

std::vector<int> PrimitiveNextConstraint::propagate(Variable &v, InstantSolver &context)
{
    return {};
}
