#include "PrimitiveNextConstraint.h"

#include "../expressions/VariableExpression.h"
#include "../Variable.h"

PrimitiveNextConstraint::PrimitiveNextConstraint(VariableExpression &variable, VariableExpression &nextVariable) :
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

// a next constraint cannot be enforced within a single timepoint, so
// propagating it should do nothing. It will only be enforced when the solver
// changes to a new timepoint and grabs the present value of mVariable, forcing it to equal
// mNextVariable in the new timepoint
std::set<int> PrimitiveNextConstraint::propagate(Variable &v, InstantSolver &context)
{
    return {};
}

bool PrimitiveNextConstraint::lt(const Constraint &rhs) const {
    if (typeid(*this).before(typeid(rhs))) {
        return true;
    } else if (typeid(*this) == typeid(rhs)) {
        const PrimitiveNextConstraint &p = static_cast<const PrimitiveNextConstraint&>(rhs);
        if (mVariable < p.mVariable) {
            return true;
        } else if (mVariable == p.mVariable) {
            return mNextVariable < p.mNextVariable;
        }
    }
    return false;
}
bool PrimitiveNextConstraint::eq(const Constraint &rhs) const {
    if (typeid(*this) == typeid(rhs)) {
        const PrimitiveNextConstraint &p = static_cast<const PrimitiveNextConstraint&>(rhs);
        return (mVariable == p.mVariable) && (mNextVariable == p.mNextVariable);
    }
    return false;
}
