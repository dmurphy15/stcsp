#include "PrimitiveNextConstraint.h"

#include "../expressions/VariableExpression.h"

PrimitiveNextConstraint::PrimitiveNextConstraint(VariableExpression &variable, VariableExpression &nextVariable) :
        Constraint({a, b}),
        mVariable(a),
        mNextVariable(b) {}

PrimitiveNextConstraint::~PrimitiveNextConstraint() {}

void PrimitiveNextConstraint::normalize(std::set<Constraint_r> &constraintList,
                                std::set<Variable_r> &variableList) const
{
    constraintList.insert(*new PrimitiveNextConstraint(*new VariableExpression(mVariable),
                                                       *new VariableExpression(mNextVariable)));
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
