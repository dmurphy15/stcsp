#pragma once

#include "../Expression.h"

class LEQExpression : public Expression
{
public:
    LEQExpression(Expression &a, Expression &b);

    int evaluate(SearchNode &context, int time) const override;

    std::set<Variable_r> getVariables() const override;

    Expression& normalize(std::set<Constraint_r> &constraintList,
                          std::set<Variable_r> &variableList) override;
    domain_t getDomain(SearchNode &context, int time) const override;
    domain_t getInitialDomain() const override;
private:
    bool lt(const Expression &rhs) const override {
        return (typeid(*this).before(typeid(rhs))) ||
               ((typeid(*this) == typeid(rhs)) && (mExpr1 < static_cast<const LEQExpression&>(rhs).mExpr1)) ||
                ((typeid(*this) == typeid(rhs)) && (mExpr1 == static_cast<const LEQExpression&>(rhs).mExpr1) && (mExpr2 < static_cast<const LEQExpression&>(rhs).mExpr2));
    }
    bool eq(const Expression &rhs) const override {
        return (typeid(*this) == typeid(rhs)) &&
               (mExpr1 == static_cast<const LEQExpression&>(rhs).mExpr1) &&
                (mExpr2 == static_cast<const LEQExpression&>(rhs).mExpr2);
    }
    Expression &mExpr1;
    Expression &mExpr2;
};