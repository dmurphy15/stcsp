#include "../../../include/constraints/specialConstraints/PrimitiveAtConstraint.h"

#include "../../../include/expressions/specialExpressions/VariableExpression.h"
#include "../../../include/expressions/specialExpressions/ConstantExpression.h"
#include "../../../include/constraints/specialConstraints/PrimitiveFirstConstraint.h"
#include "../../../include/Variable.h"
#include "../../../include/SearchNode.h"

PrimitiveAtConstraint::PrimitiveAtConstraint(VariableExpression &varExpr, Expression &expr, ConstantExpression &c) :
        Constraint({varExpr, expr, c}, false),
        mVarExpr(varExpr),
        mExpr(expr),
        mConstExpr(c) {}

PrimitiveAtConstraint::~PrimitiveAtConstraint() {}

void PrimitiveAtConstraint::normalize(std::set<Constraint_r> &constraintList,
                                        std::set<Variable_r> &variableList)
{
    throw std::logic_error("this should have only been produced through normalization, so the contents should have already been normalized");
}

bool PrimitiveAtConstraint::isSatisfied(SearchNode &context, int time) const
{
    // the At constraint will only be enforced when its time is up, at which point the solver will
    // convert it to a first constraint, so it should always defer to that
    // of course, we could rule things out to ensure that it could possibly be satisfied, but idk for sure what the
    // speedup would be from doing that sort of pruning
    return true;
}

// if we had non-naively evaluated isSatisfied, then we could have pruned the domains here
std::vector<std::set<int>> PrimitiveAtConstraint::propagate(Variable &v, SearchNode &context)
{
    std::vector<std::set<int>> ret(context.getPrefixK());
    return ret;
}

Constraint& PrimitiveAtConstraint::makeDecrementedCopy()
{
    if (mConstExpr.mConstant == 1) {
        return *new PrimitiveFirstConstraint(mVarExpr, mExpr);
    }
    return *new PrimitiveAtConstraint(mVarExpr, mExpr, *new ConstantExpression(mConstExpr.mConstant - 1));
}
