#include "../../include/constraints/LEConstraint.h"

#include <stdexcept>

#include "../../include/Variable.h"
#include "../../include/Expression.h"

#include "../../include/constraints/specialConstraints/EqualConstraint.h"
#include "../../include/expressions/specialExpressions/ConstantExpression.h"

#include "../../include/expressions/LEExpression.h"

#include "../../include/SearchNode.h"

LEConstraint::LEConstraint(Expression &a, Expression &b) :
        Constraint({a, b}, false),
        mExpr1(a),
        mExpr2(b) {}

LEConstraint::~LEConstraint() {}

bool LEConstraint::isSatisfied(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) <= mExpr2.evaluate(context, time);
}

std::vector<std::set<int>> LEConstraint::propagate(Variable &v, SearchNode &context)
{
    return context.defaultPropagate(v, *this);
}
