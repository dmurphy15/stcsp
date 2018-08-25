#include "../../include/constraints/GTConstraint.h"

#include <stdexcept>

#include "../../include/Variable.h"
#include "../../include/Expression.h"

#include "../../include/constraints/specialConstraints/EqualConstraint.h"
#include "../../include/expressions/specialExpressions/ConstantExpression.h"

#include "../../include/expressions/GTExpression.h"

#include "../../include/SearchNode.h"

GTConstraint::GTConstraint(Expression &a, Expression &b) :
        Constraint({a, b}, false),
        mExpr1(a),
        mExpr2(b) {}

GTConstraint::~GTConstraint() {}

bool GTConstraint::isSatisfied(SearchNode &context, int time) const {
    return mExpr1.evaluate(context, time) > mExpr2.evaluate(context, time);
}

std::vector<std::set<int>> GTConstraint::propagate(Variable &v, SearchNode &context)
{
    return context.defaultPropagate(v, *this);
}
