#include "../../../include/expressions/specialExpressions/FirstExpression.h"

#include "../../../include/SearchNode.h"
#include "../../../include/expressions/specialExpressions/ConstantExpression.h"
#include "../../../include/constraints/specialConstraints/EqualConstraint.h"

FirstExpression::FirstExpression(Expression &a) : Expression({a}, false), mExpr(a) {}

int FirstExpression::evaluate(SearchNode &context, int time) const
{
    return mExpr.evaluate(context, 0);
}

domain_t FirstExpression::getDomain(SearchNode &context, int time) const
{
    return mExpr.getDomain(context, 0);
}

domain_t FirstExpression::getInitialDomain() const
{
    return mExpr.getInitialDomain();
}

bool FirstExpression::containsFirstExpression() {
    return true;
}

Expression& FirstExpression::freezeFirstExpressions(SearchNode& rootNode) {
    return *new ConstantExpression(evaluate(rootNode, 0));
}
