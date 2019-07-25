#include "../../../include/expressions/specialExpressions/FirstExpression.h"

#include "../../../include/SearchNode.h"
#include "../../../include/expressions/specialExpressions/ConstantExpression.h"
#include "../../../include/constraints/specialConstraints/EqualConstraint.h"

FirstExpression::FirstExpression(Expression &a) : Expression({a}, false), mExpr(a) {}

int FirstExpression::evaluate(SearchNode &context, int time) const
{
    return mExpr.evaluate(context, 0);
}

Expression& FirstExpression::normalize(std::set<Constraint_r> &constraintList,
                                     std::map<Expression_r, Expression_r> &normalizedMap,
                                     std::set<Variable_r> &variableList)
{
    auto it = normalizedMap.find(*this);
    if (it != normalizedMap.end()) {
        return it->second;
    }

    Expression& e = mExpr.normalize(constraintList, normalizedMap, variableList);
    if (typeid(e) == typeid(ConstantExpression)) { // first(constant) is the same as constant
        normalizedMap.insert({*this, e});
        normalizedMap.insert({e, e});
        return e;
    } else {
        Expression& ret = build({e});
        normalizedMap.insert({*this, ret});
        normalizedMap.insert({ret, ret});
        return ret;
    }
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
