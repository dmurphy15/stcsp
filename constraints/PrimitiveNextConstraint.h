//
//
//
// this is a special constraint used by Solver class; there's really no need to use it elsewhere.
// produced when you normalize a next expression
//
//
//

#pragma once

#include "../Constraint.h"

class Variable;
class VariableExpression;


class PrimitiveNextConstraint : public Constraint
{
public:
    PrimitiveNextConstraint(VariableExpression &variable, VariableExpression &nextVariable);
    ~PrimitiveNextConstraint();

    void normalize(std::set<Constraint_r> &constraintList,
                   std::set<Variable_r> &variableList) override;

    int isSatisfied(InstantSolver &context) const override;

    std::set<Variable_r> getVariables() const override;

    std::vector<int> propagate(Variable &v, InstantSolver &context) override;

    Variable &mVariable;
    Variable &mNextVariable;
};