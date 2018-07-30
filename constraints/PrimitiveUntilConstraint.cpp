#include "PrimitiveUntilConstraint.h"

#include "../expressions/VariableExpression.h"
#include "../Variable.h"

PrimitiveUntilConstraint::PrimitiveUntilConstraint(VariableExpression &variable, VariableExpression &untilVariable) :
        Constraint({variable, untilVariable}),
        mVariable(*variable.getVariables().begin()),
        mUntilVariable(*untilVariable.getVariables().begin()) {}

PrimitiveUntilConstraint::~PrimitiveUntilConstraint() {}

void PrimitiveUntilConstraint::normalize(std::set<Constraint_r> &constraintList,
                                        std::set<Variable_r> &variableList) const
{
    constraintList.insert(*new PrimitiveUntilConstraint(*new VariableExpression(mVariable),
                                                       *new VariableExpression(mUntilVariable)));
}

int PrimitiveUntilConstraint::isSatisfied(InstantSolver &context) const
{
    return (mUntilVariable.evaluate(context) != 0) || (mVariable.evaluate(context) == 0);
}

std::set<Variable_r> PrimitiveUntilConstraint::getVariables() const
{
    return {mVariable, mUntilVariable};
}

std::vector<int> PrimitiveUntilConstraint::propagate(Variable &v, InstantSolver &context)
{
    return {};
}