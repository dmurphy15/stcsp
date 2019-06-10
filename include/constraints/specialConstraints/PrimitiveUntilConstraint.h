//
//
//
// special constraint for until constraints
//
//
//
//

#pragma once

#include "../../Constraint.h"

class Variable;
class VariableExpression;


class PrimitiveUntilConstraint : public Constraint
{
public:
    PrimitiveUntilConstraint(VariableExpression &variable, VariableExpression &untilVariable, int expressionSetId=-1);
    ~PrimitiveUntilConstraint();

    void normalize(std::set<Constraint_r> &constraintList,
                   std::set<Variable_r> &variableList) override;

    bool isSatisfied(SearchNode &context, int time) const override;
    std::vector<std::set<int>> propagate(Variable &v, SearchNode &context) override;

    Variable &mVariable;
    Variable &mUntilVariable;
};