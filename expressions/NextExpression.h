#pragma once

#include "../Expression.h"

class NextExpression : public Expression
{
public:
    NextExpression(Expression &a);

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
                (mExpr < static_cast<const NextExpression&>(rhs).mExpr));
    }
    bool eq(const Expression &rhs) const override {
        return (typeid(*this) == typeid(rhs)) &&
               (mExpr == static_cast<const NextExpression&>(rhs).mExpr);
    }
    Expression &mExpr;
};