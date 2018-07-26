#pragma once

#include "../Constraint.h"

class EqualConstraint : public Constraint
{
public:
    EqualConstraint(Expression *a, Expression *b);
    ~EqualConstraint();

    void normalize(std::set<Constraint *, ConstraintLtComparator> *constraintList,
                   std::set<Variable *> *variableList) const override;

    int isSatisfied(InstantaneousCSP *context) const override;

    std::set<Variable *> getVariables() const override;

    bool propagate(Variable *v, InstantaneousCSP *context) override;
private:
    Expression *mExpr1;
    Expression *mExpr2;
};