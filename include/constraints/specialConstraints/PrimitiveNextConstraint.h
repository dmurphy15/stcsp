#pragma once

#include "../../Constraint.h"
#include "../../expressions/specialExpressions/VariableExpression.h"

/**
 * used to represent A == next B, where both A and B are known to be VariableExpressions. Produced when normalizing
 * nextExpressions.
 *
 * Currently, my propagation algorithm is a Bounds Consistency version for this, but you can find a GAC version
 * commented out in the .cpp file
 *
 * Between searchnodes, the solver will check for NextConstraints, and use them to produce historicalValues to pass
 * on and constrain variables in the next searchnode
 */

class PrimitiveNextConstraint : public Constraint
{
public:
    PrimitiveNextConstraint(VariableExpression &varExpr, VariableExpression &nextVarExpr, int expressionSetId=-1);
    ~PrimitiveNextConstraint();

    void normalize(std::set<Constraint_r> &constraintList,
                   std::map<Expression_r, Expression_r> &normalizedMap,
                   std::set<Variable_r> &variableList) override;

    bool isSatisfied(SearchNode &context, int time) const override;
    std::map<Variable_r, std::vector<std::set<int>>> propagate(SearchNode &context) override;

    VariableExpression &mVarExpr;
    VariableExpression &mNextVarExpr;
    Variable &mVariable;
    Variable &mNextVariable;
private:
    void propagateHelper(bool vIsNext,
                         SearchNode &context,
                         std::vector<std::set<int>>& ret);
};