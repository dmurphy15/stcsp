#include "EqualConstraint.h"

#include <stdexcept>

#include "../Expression.h"


EqualConstraint::EqualConstraint(Expression *a, Expression *b) : mExpr1(a), mExpr2(b) {
    init();
}

EqualConstraint::~EqualConstraint() {
    deinit();
}

void EqualConstraint::normalize(std::unordered_set<Constraint *> *const constraintList,
               std::unordered_set<Variable *> *const variableList) const
{
    constraintList->insert(new EqualConstraint(mExpr1, mExpr2));
}

int EqualConstraint::isSatisfied(int time) const
{
    return mExpr1->evaluate(time) == mExpr2->evaluate(time);
}

std::unordered_set<Variable *> EqualConstraint::getVariables() const
{
    std::unordered_set<Variable *> vars1 = mExpr1->getVariables();
    std::unordered_set<Variable *> vars2 = mExpr2->getVariables();
    vars1.insert(vars2.begin(), vars2.end());
    return vars1;
}

bool EqualConstraint::overrideDefaultPropagation() const
{
    return false;
}

std::unordered_set<Constraint *> EqualConstraint::propagate(int time, Variable *v) const
{
    return mOwnerSolver->defaultPropagate(time, v);
//    throw std::logic_error((std::string)__PRETTY_FUNCTION__ + " should have been overridden");
//    return {};
//        std::set<Variable *> vars = getVariables();
    /*
     * check that v is in vars
     * iterate through the domain of v
     * iterate through the domains of all other vars, assigning them
     *  to each permutation and then calling isSatisfied
     * if it is satisfied for a particular value of v, stop
     * if for a value of v it is never satisfied, prune that value
     * at the end, if any values were pruned, return getConstraints()
     */
}
