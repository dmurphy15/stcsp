#pragma once

#include "../Expression.h"

class AddExpression : public Expression
{
public:
    AddExpression(Expression &a, Expression &b);

    int evaluate(SearchNode &context, int time) const override;

    std::set<Variable_r> getVariables() const override;

    Expression& normalize(std::set<Constraint_r> &constraintList,
                           std::set<Variable_r> &variableList) override;
    std::set<int> getDomain(SearchNode &context, int time) const override;
    std::set<int> getInitialDomain() const override;
private:
    bool lt(const Expression &rhs) const override {
        return (typeid(*this).before(typeid(rhs))) ||
               ((typeid(*this) == typeid(rhs)) &&
                (mExpressions < static_cast<const AddExpression&>(rhs).mExpressions));
    }
    bool eq(const Expression &rhs) const override {
        return (typeid(*this) == typeid(rhs)) &&
               (mExpressions == static_cast<const AddExpression&>(rhs).mExpressions);
    }
    Expression &mExpr1;
    Expression &mExpr2;
    // used to speed up comparison, since this expression is symmetric
    std::set<Expression_r> mExpressions;
};