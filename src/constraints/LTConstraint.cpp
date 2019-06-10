#include "../../include/constraints/LTConstraint.h"

#include <stdexcept>

#include "../../include/Variable.h"
#include "../../include/Expression.h"

#include "../../include/constraints/specialConstraints/EqualConstraint.h"
#include "../../include/expressions/specialExpressions/ConstantExpression.h"

#include "../../include/expressions/LTExpression.h"

#include "../../include/SearchNode.h"

LTConstraint::LTConstraint(Expression &a, Expression &b, int expressionSetId) :
        Constraint({a, b}, false, expressionSetId),
        mExpr1(a),
        mExpr2(b) {}

LTConstraint::~LTConstraint() {}

bool LTConstraint::isSatisfied(SearchNode &context, int time) const
{
    return mExpr1.evaluate(context, time) < mExpr2.evaluate(context, time);
}

std::vector<std::set<int>> LTConstraint::propagate(Variable &v, SearchNode &context)
{
    return context.defaultPropagate(v, *this);
}
