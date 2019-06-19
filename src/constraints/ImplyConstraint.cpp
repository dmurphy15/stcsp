#include "../../include/constraints/ImplyConstraint.h"

#include "../../include/Expression.h"
#include "../../include/Variable.h"

#include "../../include/SearchNode.h"

ImplyConstraint::ImplyConstraint(Expression &a, Expression &b, int expressionSetId) :
        Constraint({a, b}, false, expressionSetId),
        mExpr1(a),
        mExpr2(b) {}

ImplyConstraint::~ImplyConstraint() {}

bool ImplyConstraint::isSatisfied(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) == 0 || mExpr2.evaluate(context, time) != 0;
}

std::map<Variable_r, std::vector<std::set<int>>> ImplyConstraint::propagate(SearchNode &context)
{
    return context.defaultPropagate(*this);
}