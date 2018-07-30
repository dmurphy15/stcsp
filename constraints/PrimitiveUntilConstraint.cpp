#include "PrimitiveUntilConstraint.h"

#include "../expressions/VariableExpression.h"

PrimitiveUntilConstraint::PrimitiveUntilConstraint(VariableExpression &variable, VariableExpression &untilVariable) :
        Constraint({a, b}),
        mVariable(a),
        mUntilVariable(b) {}

PrimitiveUntilConstraint::~PrimitiveUntilConstraint() {}

void PrimitiveUntilConstraint::normalize(std::set<Constraint_r> &constraintList,
                                        std::set<Variable_r> &variableList) const
{
    constraintList.insert(*new PrimitiveUntilConstraint(*new VariableExpression(mVariable),
                                                       *new VariableExpression(mUntilVariable)));
}

int PrimitiveUntilConstraint::isSatisfied(InstantSolver &context) const
{
    return (mUntilVariable != 0) || (mVariable == 0);
}

std::set<Variable_r> PrimitiveUntilConstraint::getVariables() const
{
    return {mVariable, mUntilVariable};
}

std::vector<int> PrimitiveUntilConstraint::propagate(Variable &v, InstantSolver &context)
{
    return {};
}
