#pragma once

#include <unordered_set>

#include "../Constraint.h"

class Expression;
class Variable;




class EqualConstraint : Constraint
{
public:
    EqualConstraint(Expression *a, Expression *b);
    ~EqualConstraint();

    void normalize(std::unordered_set<Constraint *> *const constraintList,
                   std::unordered_set<Variable *> *const variableList) const override;

    int isSatisfied(int time) const override;

    std::unordered_set<Variable *> getVariables() const override;

    bool overrideDefaultPropagation() const override;

    std::unordered_set<Constraint *> propagate(int time, Variable *v) const override;
private:
    Expression *mExpr1;
    Expression *mExpr2;
};