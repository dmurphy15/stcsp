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
public:
    PrimitiveUntilConstraint(VariableExpression &variable, VariableExpression &untilVariable);
    ~PrimitiveUntilConstraint();

    void normalize(std::set<Constraint_r> &constraintList,
                   std::set<Variable_r> &variableList) override;

    int isSatisfied(InstantSolver &context) const override;

    std::set<Variable_r> getVariables() const override;

    std::set<int> propagate(Variable &v, InstantSolver &context) override;

    Variable &mVariable;
    Variable &mUntilVariable;
private:
    bool lt(const Constraint &rhs) const override;
    bool eq(const Constraint &rhs) const override;
};