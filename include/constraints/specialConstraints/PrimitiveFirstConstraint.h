#pragma once

#include "../../Constraint.h"

class Variable;
class VariableExpression;

/**
 * used to represent something similar to A == first B, where A is known to be a variableExpression and B is an
 * Expression. Importantly, this is more flexible than A == first B, in that if we are at timepoint t, we
 * are treating first B as "the value B takes at the timepoint t", so it doesn't have to be in the root node. This is
 * needed to effectively propagate PrimitiveAtConstraints.
 *
 * When creating each new searchnode, the solver will check its constraint set for PrimitiveFirstConstraints and
 * remove them, since they will no longer be needed for propagation past one timepoint (since anything constrained by a
 * PrimitiveFirstConstraint should also have been constrained with A == next A if we want it to maintain a constant value)
 */

class PrimitiveFirstConstraint : public Constraint
{
public:
    PrimitiveFirstConstraint(VariableExpression &variableExpr, Expression &firstExpr, int expressionSetId=-1);
    ~PrimitiveFirstConstraint();

    void normalize(std::set<Constraint_r> &constraintList,
                   std::map<Expression_r, Expression_r> &normalizedMap,
                   std::set<Variable_r> &variableList) override;

    bool isSatisfied(SearchNode &context, int time) const override;
    std::map<Variable_r, std::vector<std::set<int>>> propagate(SearchNode &context) override;

    VariableExpression &mVariableExpr;
    Expression &mFirstExpr;
    Variable &mVariable;
private:
    bool shouldPrune(SearchNode& context, std::vector<Variable_r>& vars, std::vector<Variable_r>::iterator index);
};