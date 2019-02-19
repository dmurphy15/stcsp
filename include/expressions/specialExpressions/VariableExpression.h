#pragma once

#include "../../Expression.h"

/*
 * Class for a an Expression that holds a single Variable. Used to separate
 * Expression attributes/methods from Variable attributes/methods. Also makes
 * parsing easier, so we can make multiple references to the same underlying variable.
 */
class VariableExpression : public Expression
{
public:
    VariableExpression(Variable &v);

    int evaluate(SearchNode &context, int time) const override;

    domain_t getDomain(SearchNode &context, int time) const override;
    domain_t getInitialDomain() const override;
    void getVariables(std::set<Variable_r>& variables, bool root = true) const override;
    Variable &mVariable;
private:
    bool lt(const Expression &rhs) const override;
    bool eq(const Expression &rhs) const override;
    Expression& build(std::vector<Expression_r>& expressions) override {
        return *new VariableExpression(mVariable);
    }
};