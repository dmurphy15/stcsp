#pragma once

#include "../../Constraint.h"

class Variable;
class VariableExpression;
class ConstantExpression;

/**
 * used to represent A == B @ t, where A is known to be a variableExpression, B is an Expression, and t is a
 * constantExpression. Evaluated lazily; it does not propagate until we reach timepoint t, at which point this will
 * transform into a PrimitiveFirstExpression.
 *
 * Between searchnodes, the solver will call makeDecrementedCopy to update this constraint at each timpoint, until
 * we reach time t, at which point this constraint will transform into a PrimitiveFirstExpression.
 */

class PrimitiveAtConstraint : public Constraint
{
public:
    PrimitiveAtConstraint(VariableExpression &varExpr, Expression &expr, ConstantExpression &c, int expressionSetId=-1);
    ~PrimitiveAtConstraint();

    void normalize(std::set<Constraint_r> &constraintList,
                   std::map<Expression_r, Expression_r> &normalizedMap,
                   std::set<Variable_r> &variableList) override;

    bool isSatisfied(SearchNode &context, int time) const override;
    std::map<Variable_r, std::vector<std::set<int>>> propagate(SearchNode &context) override;

    Constraint& makeDecrementedCopy();

    VariableExpression &mVarExpr;
    Expression &mExpr;
    ConstantExpression &mConstExpr;
};