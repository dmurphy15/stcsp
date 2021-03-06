#pragma once

#include "../../Expression.h"

/**
 * Class for a an Expression whose value is constant.
 */

class ConstantExpression : public Expression
{
public:
    ConstantExpression(int constant);

    int evaluate(SearchNode &context, int time) const override;

    domain_t getDomain(SearchNode &context, int time) const override;
    domain_t getInitialDomain() const override;
    std::set<Variable_r> getVariables() const override;
    const int mConstant;
private:
    bool lt(const Expression &rhs) const override {
        return (typeid(*this).before(typeid(rhs))) ||
               ((typeid(*this) == typeid(rhs)) &&
                (mConstant < static_cast<const ConstantExpression&>(rhs).mConstant));
    }
    bool eq(const Expression &rhs) const override {
        return (typeid(*this) == typeid(rhs)) &&
               (mConstant == static_cast<const ConstantExpression&>(rhs).mConstant);
    }
    Expression& build(const std::vector<Expression_r>& expressions) override {
        return *new ConstantExpression(mConstant);
    }
};