#pragma once

#include "../../Constraint.h"

class Variable;
class VariableExpression;

/**
 * used to represent A until B, where both A and B are known to be variableExpressions. At each timepoint,
 * this enforces that: (not B) -> A, until B is non-zero, at and after which point A is no longer constrained. It also
 * requires that B be non-zero eventually, which is why the SolverPruner is needed to trim non-terminal nodes after a
 * solution graph has been constructed.
 *
 * Between searchnodes, the Solver will check whether a PrimitiveUntilConstraint has been satisfied, and if so,
 * it will remove it from the constraint set for the next searchnode.
 */

class PrimitiveUntilConstraint : public Constraint
{
public:
    PrimitiveUntilConstraint(VariableExpression &variable, VariableExpression &untilVariable, int expressionSetId=-1);
    ~PrimitiveUntilConstraint();

    void normalize(std::set<Constraint_r> &constraintList,
                   std::map<Expression_r, Expression_r> &normalizedMap,
                   std::set<Variable_r> &variableList) override;

    bool isSatisfied(SearchNode &context, int time) const override;
    std::map<Variable_r, std::vector<std::set<int>>> propagate(SearchNode &context) override;

    Variable &mVariable;
    Variable &mUntilVariable;
};