#pragma once

#include "../Expression.h"

class IfThenElseExpression : public Expression
{
public:
    IfThenElseExpression(Expression &ifExp, Expression &thenExp, Expression &elseExp);

    int evaluate(SearchNode &context, int time) const override;

    std::set<Variable_r> getVariables() const override;

    Expression& normalize(std::set<Constraint_r> &constraintList,
                          std::set<Variable_r> &variableList) override;
    domain_t getDomain(SearchNode &context, int time) const override;
    domain_t getInitialDomain() const override;
private:
    bool lt(const Expression &rhs) const override {
        return (typeid(*this).before(typeid(rhs))) ||
                    ((typeid(*this) == typeid(rhs)) &&
                     (mExpr1 < static_cast<const IfThenElseExpression&>(rhs).mExpr1)) ||
                        ((typeid(*this) == typeid(rhs)) &&
                         (mExpr1 == static_cast<const IfThenElseExpression&>(rhs).mExpr1) &&
                         (mExpr2 < static_cast<const IfThenElseExpression&>(rhs).mExpr2)) ||
                             ((typeid(*this) == typeid(rhs)) &&
                             (mExpr1 == static_cast<const IfThenElseExpression&>(rhs).mExpr1) &&
                             (mExpr2 == static_cast<const IfThenElseExpression&>(rhs).mExpr2) &&
                              (mExpr3 < static_cast<const IfThenElseExpression&>(rhs).mExpr3));
    }
    bool eq(const Expression &rhs) const override {
        return (typeid(*this) == typeid(rhs)) &&
               (mExpr1 == static_cast<const IfThenElseExpression&>(rhs).mExpr1) &&
                (mExpr2 == static_cast<const IfThenElseExpression&>(rhs).mExpr2) &&
                (mExpr3 == static_cast<const IfThenElseExpression&>(rhs).mExpr3);
    }
    Expression &mExpr1;
    Expression &mExpr2;
    Expression &mExpr3;
};