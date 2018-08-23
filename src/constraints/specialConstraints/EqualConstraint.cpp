#include "../../../include/constraints/specialConstraints/EqualConstraint.h"

#include "../../../include/SearchNode.h"
#include "../../../include/Expression.h"
#include "../../../include/Constraint.h"
#include "../../../include/Variable.h"

EqualConstraint::EqualConstraint(Expression &a, Expression &b) :
        mExpr1(a),
        mExpr2(b),
        mExpressions({a, b}) {}

EqualConstraint::~EqualConstraint() {}

void EqualConstraint::normalize(std::set<Constraint_r> &constraintList,
                                std::set<Variable_r> &variableList)
{
    Expression &equivalentExpr1 = mExpr1.normalize(constraintList, variableList);
    Expression &equivalentExpr2 = mExpr2.normalize(constraintList, variableList);
    Constraint &equivalentConstraint = *new EqualConstraint(equivalentExpr1, equivalentExpr2);
    constraintList.insert(equivalentConstraint);
}

int EqualConstraint::isSatisfied(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) == mExpr2.evaluate(context, time);
}

std::set<Variable_r> EqualConstraint::getVariables() const
{
    std::set<Variable_r> vars1 = mExpr1.getVariables();
    std::set<Variable_r> vars2 = mExpr2.getVariables();
    vars1.insert(vars2.begin(), vars2.end());
    return vars1;
}

std::vector<std::set<int>> EqualConstraint::propagate(Variable &v, SearchNode &context)
{
    //TODO in the future, could instead call some kind of set_intersection technique, like I did in primitiveNextConstraint
    //TODO then again, perhaps not, since there I already had to sacrifice the time to iterate through all assignments, so maybe this is faster
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

bool EqualConstraint::lt(const Constraint &rhs) const {
    return (typeid(*this).before(typeid(rhs))) ||
           ((typeid(*this) == typeid(rhs)) &&
            (mExpressions < static_cast<const EqualConstraint&>(rhs).mExpressions));
}
bool EqualConstraint::eq(const Constraint &rhs) const {
    return (typeid(*this) == typeid(rhs)) &&
           (mExpressions == static_cast<const EqualConstraint&>(rhs).mExpressions);
}
