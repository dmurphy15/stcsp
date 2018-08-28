#include "../../include/constraints/NEQConstraint.h"

#include <stdexcept>

#include "../../include/Variable.h"
#include "../../include/Expression.h"

#include "../../include/constraints/specialConstraints/EqualConstraint.h"
#include "../../include/expressions/specialExpressions/ConstantExpression.h"

#include "../../include/expressions/LEExpression.h"

#include "../../include/SearchNode.h"

NEQConstraint::NEQConstraint(Expression &a, Expression &b) :
        Constraint({a, b}, true),
        mExpr1(a),
        mExpr2(b) {}

NEQConstraint::~NEQConstraint() {}

bool NEQConstraint::isSatisfied(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) != mExpr2.evaluate(context, time);
}

std::vector<std::set<int>> NEQConstraint::propagate(Variable &v, SearchNode &context)
{
    return context.defaultPropagate(v, *this);
}