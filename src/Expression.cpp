#include "../include/Expression.h"

#include <algorithm>
#include <stdexcept>

#include "../include/Variable.h"

Expression::Expression(std::initializer_list<Expression_r> expressions, bool symmetric) : mExpressions(expressions) {
    if (symmetric) {
        std::sort(mExpressions.begin(), mExpressions.end());
    }
}

std::set<Variable_r> Expression::getVariables(bool root) const {
    std::set<Variable_r> ret;
    for (Expression& e : mExpressions) {
        std::set<Variable_r>&& vars = e.getVariables(root);
        auto it1 = ret.begin(); auto it2 = vars.begin();
        while (it2 != vars.end()) {
            while (it1 != ret.end() && &(*it1) <= &(*it2)) {
                it1++;
            }
            ret.insert(it1, *it2);
            it2++;
        }
    }
    return ret;
}

Expression& Expression::freezeFirstExpressions() {
    if (!mContainsFirstExpression) {
        return *this;
    }
    std::vector<Expression_r> frozen = {};
    for (Expression& e : mExpressions) {
        frozen.push_back(e.freezeFirstExpressions());
    }
    for (int i=0; i < frozen.size(); i++) {
        if (&(frozen[i].get()) != &(mExpressions[i].get())) {
            return build(frozen);
        }
    }
    mContainsFirstExpression = false;
    return *this;
}

Expression& Expression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList) {
    std::vector<Expression_r> normalized;
    for (Expression &e : mExpressions) {
        normalized.push_back(e.normalize(constraintList, variableList));
    }
    return build(normalized);
}

domain_t Expression::getDomain(SearchNode &context, int time) const {
    domain_t ret;
    std::vector<int> v(mExpressions.size());
    getDomainHelper(mExpressions, context, time, v, 0, ret);
    return ret;
}

domain_t Expression::getInitialDomain() const {
    domain_t ret;
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
                                 domain_t& accumulator) const {
    if (index == expressions.size()) {
        accumulator.insert(evaluateFake(values));
        return;
    }
    domain_t&& d = expressions[index].get().getDomain(context, time);
    for (auto it = d.begin(); it != d.end(); it++) {
        values[index] = *it;
        getDomainHelper(expressions, context, time, values, index+1, accumulator);
    }
}

void Expression::getInitialDomainHelper(const std::vector<Expression_r>& expressions,
                                 std::vector<int>& values,
                                 std::size_t index,
                                 domain_t& accumulator) const {
    if (index == expressions.size()) {
        accumulator.insert(evaluateFake(values));
        return;
    }
    domain_t&& d = expressions[index].get().getInitialDomain();
    for (auto it = d.begin(); it != d.end(); it++) {
        values[index] = *it;
        getInitialDomainHelper(expressions, values, index+1, accumulator);
    }
}