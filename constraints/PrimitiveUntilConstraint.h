//
//
//
// special constraint for until constraints
//
//
//
//

#pragma once

#include "../Constraint.h"

class Variable;
class VariableExpression;


class PrimitiveUntilConstraint : public Constraint
{
    using Variable_r = std::reference_wrapper<Variable>;
public:
    PrimitiveUntilConstraint(VariableExpression &variable, VariableExpression &untilVariable);
    ~PrimitiveUntilConstraint();

    void normalize(std::set<Constraint_r> &constraintList,
                   std::set<Variable_r> &variableList) const override;

    int isSatisfied(InstantSolver &context) const override;

    std::set<Variable_r> getVariables() const override;

    std::vector<int> propagate(Variable &v, InstantSolver &context) override;

    Variable &mVariable;
    Variable &mUntilVariable;
};