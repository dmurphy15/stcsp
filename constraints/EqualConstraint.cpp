#include "EqualConstraint.h"

#include "../Variable.h"
#include "../Expression.h"
#include "../Constraint.h"

#include "../InstantSolver.h"

EqualConstraint::EqualConstraint(Expression &a, Expression &b) :
        Constraint({a, b}),
        mExpr1(a),
        mExpr2(b) {}

EqualConstraint::~EqualConstraint() {}

void EqualConstraint::normalize(std::set<Constraint_r> &constraintList,
                                std::set<Variable_r> &variableList) const
{
    constraintList.insert(*(new EqualConstraint(mExpr1, mExpr2)));
}

int EqualConstraint::isSatisfied(InstantSolver &context) const
{
    return mExpr1.evaluate(context) == mExpr2.evaluate(context);
}

std::set<Variable_r> EqualConstraint::getVariables() const
{
    std::set<Variable_r> vars1 = mExpr1.getVariables();
    std::set<Variable_r> vars2 = mExpr2.getVariables();
    vars1.insert(vars2.begin(), vars2.end());
    return vars1;
}

std::vector<int> EqualConstraint::propagate(Variable &v, InstantSolver &context)
{
    return context.defaultPropagate(v, *this);
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
