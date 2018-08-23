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
    PrimitiveFirstConstraint(VariableExpression &variableExpr, Expression &firstExpr);
    ~PrimitiveFirstConstraint();

    void normalize(std::set<Constraint_r> &constraintList,
                   std::set<Variable_r> &variableList) override;

    int isSatisfied(SearchNode &context, int time) const override;

    std::set<Variable_r> getVariables() const override;

    std::vector<std::set<int>> propagate(Variable &v, SearchNode &context) override;

    VariableExpression &mVariableExpr;
    Expression &mFirstExpr;
    Variable &mVariable;
private:
    bool lt(const Constraint &rhs) const override;
    bool eq(const Constraint &rhs) const override;

    bool shouldPrune(SearchNode& context, std::vector<Variable_r>& vars, int index);

};