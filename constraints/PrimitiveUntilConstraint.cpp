#include "PrimitiveUntilConstraint.h"

#include "../expressions/VariableExpression.h"
#include "../Variable.h"
#include "../InstantSolver.h"

PrimitiveUntilConstraint::PrimitiveUntilConstraint(VariableExpression &variable, VariableExpression &untilVariable) :
        mVariable(*variable.getVariables().begin()),
        mUntilVariable(*untilVariable.getVariables().begin()) {}

PrimitiveUntilConstraint::~PrimitiveUntilConstraint() {}

void PrimitiveUntilConstraint::normalize(std::set<Constraint_r> &constraintList,
                                        std::set<Variable_r> &variableList)
{
    // can do this bc normalizing the member variable expressions would do nothing anyway
    constraintList.insert(*this);
}

int PrimitiveUntilConstraint::isSatisfied(InstantSolver &context) const
{
    return (mVariable.evaluate(context) != 0) || (mUntilVariable.evaluate(context) != 0);
}

std::set<Variable_r> PrimitiveUntilConstraint::getVariables() const
{
    return {mVariable, mUntilVariable};
}

std::set<int> PrimitiveUntilConstraint::propagate(Variable &v, InstantSolver &context)
{
    return context.defaultPropagate(v, *this);
}

bool PrimitiveUntilConstraint::lt(const Constraint &rhs) const {
    if (typeid(*this).before(typeid(rhs))) {
        return true;
    } else if (typeid(*this) == typeid(rhs)) {
        const PrimitiveUntilConstraint &p = static_cast<const PrimitiveUntilConstraint&>(rhs);
        if (mVariable < p.mVariable) {
            return true;
        } else if (mVariable == p.mVariable) {
            return mUntilVariable < p.mUntilVariable;
        }
    }
    return false;
}
bool PrimitiveUntilConstraint::eq(const Constraint &rhs) const {
    if (typeid(*this) == typeid(rhs)) {
        const PrimitiveUntilConstraint &p = static_cast<const PrimitiveUntilConstraint&>(rhs);
        return (mVariable == p.mVariable) && (mUntilVariable == p.mUntilVariable);
    }
    return false;
}
