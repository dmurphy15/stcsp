#include "../../include/constraints/ImplyConstraint.h"

#include "../../include/Expression.h"
#include "../../include/Variable.h"

#include "../../include/SearchNode.h"

ImplyConstraint::ImplyConstraint(Expression &a, Expression &b) :
        Constraint({a, b}, false),
        mExpr1(a),
        mExpr2(b) {}

ImplyConstraint::~ImplyConstraint() {}

bool ImplyConstraint::isSatisfied(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) == 0 || mExpr2.evaluate(context, time) != 0;
}

std::vector<std::set<int>> ImplyConstraint::propagate(Variable &v, SearchNode &context)
{
    return context.defaultPropagate(v, *this);
}