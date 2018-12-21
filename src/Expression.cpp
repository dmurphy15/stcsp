#include "../include/Expression.h"

#include <algorithm>
#include <stdexcept>

#include "../include/Variable.h"
#include "../include/Domain.h"

Expression::Expression(std::initializer_list<Expression_r> expressions, bool symmetric) : mExpressions(expressions) {
    if (symmetric) {
        std::sort(mExpressions.begin(), mExpressions.end());
    }
}

void Expression::getVariables(std::set<Variable_r>& variables) const {
    for (Expression& e : mExpressions) {
        e.getVariables(variables);
    }
}

Expression& Expression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList) {
    std::vector<Expression_r> normalized;
    for (Expression &e : mExpressions) {
        normalized.push_back(e.normalize(constraintList, variableList));
    }
    return build(normalized);
}

Domain Expression::getDomain(SearchNode &context, int time) const {
    Domain ret;
    std::vector<int> v(mExpressions.size());
    getDomainHelper(mExpressions, context, time, v, 0, ret);
    return ret;
}

Domain Expression::getInitialDomain() const {
    Domain ret;
    std::vector<int> v(mExpressions.size());
    getInitialDomainHelper(mExpressions, v, 0, ret);
    return ret;
}

Expression& Expression::build(std::vector <Expression_r> &expressions) {
    throw std::logic_error(std::string(__FUNCTION__) + " was not implemented\n");
}

int Expression::evaluateFake(const std::vector<int> &values) const {
    throw std::logic_error(std::string(__FUNCTION__) + " was not implemented\n");
}

void Expression::getDomainHelper(const std::vector<Expression_r>& expressions,
                                 SearchNode &context,
                                 int time,
                                 std::vector<int>& values,
                                 std::size_t index,
                                 Domain& accumulator) const {
    if (index == expressions.size()) {
        accumulator.insert(evaluateFake(values));
        return;
    }
    Domain&& d = expressions[index].get().getDomain(context, time);
    for (auto it = d.begin(); it != d.end(); it++) {
        values[index] = *it;
        getDomainHelper(expressions, context, time, values, index+1, accumulator);
    }
}

void Expression::getInitialDomainHelper(const std::vector<Expression_r>& expressions,
                                 std::vector<int>& values,
                                 std::size_t index,
                                 Domain& accumulator) const {
    if (index == expressions.size()) {
        accumulator.insert(evaluateFake(values));
        return;
    }
    Domain&& d = expressions[index].get().getInitialDomain();
    for (auto it = d.begin(); it != d.end(); it++) {
        values[index] = *it;
        getInitialDomainHelper(expressions, values, index+1, accumulator);
    }
}