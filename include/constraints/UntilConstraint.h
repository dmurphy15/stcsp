#pragma once

#include "../Constraint.h"

/**
 * represents A until B. Gets normalized with PrimitiveUntilConstraints. At each timepoint,
 * this enforces that: (not B) -> A, until B is non-zero, at and after which point A is no longer constrained. It also
 * requires that B be non-zero eventually, which is why the SolverPruner is needed to trim non-terminal nodes after a
 * solution graph has been constructed.
 */

class UntilConstraint : public Constraint
{
public:
    UntilConstraint(Expression &a, Expression &b, int expressionSetId=-1);
    ~UntilConstraint();

    void normalize(std::set<Constraint_r> &constraintList,
                   std::map<Expression_r, Expression_r> &normalizedMap,
                   std::set<Variable_r> &variableList) override;

    bool isSatisfied(SearchNode &context, int time) const override;
    std::map<Variable_r, std::vector<std::set<int>>> propagate(SearchNode &context) override;
private:
    Expression &mExpr;
    Expression &mUntilExpr;
};