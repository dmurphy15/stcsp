#pragma once
//
//
//
// special constraint for First constraints
// we need this because currently our First Expressions assume that we are referring to the VERY FIRST
// timepoint we ever consider, whereas the AT constraint requires us to use the first value of
// some variables AFTER A CERTAIN AMOUNT OF TIME has passed
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
    std::map<Variable_r, std::vector<std::set<int>>> propagate(SearchNode &context) override;

    VariableExpression &mVariableExpr;
    Expression &mFirstExpr;
    Variable &mVariable;
private:
    bool shouldPrune(SearchNode& context, std::vector<Variable_r>& vars, std::vector<Variable_r>::iterator index);
};