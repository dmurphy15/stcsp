#pragma once
//
//
//
// special constraint for First constraints
//
//
//
//



#include "../../Constraint.h"

class Variable;
class VariableExpression;


class PrimitiveFirstConstraint : public Constraint
{
public:
    PrimitiveFirstConstraint(VariableExpression &variableExpr, Expression &firstExpr, int expressionSetId=-1);
    ~PrimitiveFirstConstraint();

    void normalize(std::set<Constraint_r> &constraintList,
                   std::set<Variable_r> &variableList) override;

    bool isSatisfied(SearchNode &context, int time) const override;
    std::vector<std::set<int>> propagate(Variable &v, SearchNode &context) override;

    VariableExpression &mVariableExpr;
    Expression &mFirstExpr;
    Variable &mVariable;
private:
    bool shouldPrune(SearchNode& context, std::set<Variable_r>& vars, std::set<Variable_r>::iterator index);
};