#pragma once

#include "../../Constraint.h"

class Variable;
class VariableExpression;
class ConstantExpression;


class PrimitiveAtConstraint : public Constraint
{
public:
    PrimitiveAtConstraint(VariableExpression &varExpr, Expression &expr, ConstantExpression &c, int expressionSetId=-1);
    ~PrimitiveAtConstraint();

    void normalize(std::set<Constraint_r> &constraintList,
                   std::set<Variable_r> &variableList) override;

    bool isSatisfied(SearchNode &context, int time) const override;
    std::vector<std::set<int>> propagate(Variable &v, SearchNode &context) override;

    Constraint& makeDecrementedCopy();

    VariableExpression &mVarExpr;
    Expression &mExpr;
    ConstantExpression &mConstExpr;
};