//
//
//
// this is a special constraint used by Solver class; there's really no need to use it elsewhere.
// ACTUALLY YOU SHOULD NEVER USE IT ELSEWHERE, (SINCE IT MAKES ASSUMPTIONS ABOUT MVARIABLEEXPRESSION NEVER APPEARING INSIDE MNEXTEXPRESSION)
// produced when you normalize a next expression
//
// actually nevermind now
//

//// just reminding you again, the current implementation of propagate would need some kind of while loop if we were to
//// allow for the case where mVarExpr is also mNextVarExpr

#pragma once

#include "../Constraint.h"
#include "../specialExpressions/VariableExpression.h"

class PrimitiveNextConstraint : public Constraint
{
public:
    PrimitiveNextConstraint(VariableExpression &varExpr, VariableExpression &nextVarExpr);
    ~PrimitiveNextConstraint();

    void normalize(std::set<Constraint_r> &constraintList,
                   std::set<Variable_r> &variableList) override;

    int isSatisfied(SearchNode &context, int time) const override;

    std::set<Variable_r> getVariables() const override;

    std::vector<std::set<int>> propagate(Variable &v, SearchNode &context) override;

    VariableExpression &mVarExpr;
    VariableExpression &mNextVarExpr;
    Variable &mVariable;
    Variable &mNextVariable;
private:
    bool lt(const Constraint &rhs) const override;
    bool eq(const Constraint &rhs) const override;

    void propagateHelper(Variable &v,
                         SearchNode &context,
                         bool vIsNext,
                         std::vector<std::set<int>>& ret);
};