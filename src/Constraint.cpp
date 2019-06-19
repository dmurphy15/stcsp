#include "../include/Constraint.h"

#include <algorithm>
#include <stdexcept>

#include "../include/SetRegistry.h"
#include "../include/Variable.h"
#include "../include/Expression.h"

Constraint::Constraint(std::initializer_list<Expression_r> expressions,
                       bool symmetric,
                       int expressionSetId) : mExpressions(expressions) {
    if (symmetric) {
        std::sort(mExpressions.begin(), mExpressions.end());
    }
    if (expressionSetId < 0) {
        mExpressionSetId = SetRegistry::GetExpressionSetId(mExpressions);
    } else {
        mExpressionSetId = expressionSetId;
    }
    mVariablesAtRoot = getVariables(true);
    mVariablesAfterRoot = getVariables(false);
}

std::set<Variable_r> Constraint::getVariables(bool root) const {
    if (root && !mVariablesAtRoot.empty()) {
        return mVariablesAtRoot;
    } else if (!mVariablesAfterRoot.empty()){
        return mVariablesAfterRoot;
    }
    std::set<Variable_r> ret;
    for (Expression& e : mExpressions) {
        std::set<Variable_r>&& vars = e.getVariables(root);
        auto it1 = ret.begin(); auto it2 = vars.begin();
        while (it2 != vars.end()) {
            while (it1 != ret.end() && &(*it1) < &(*it2)) {
                it1++;
            }
            ret.insert(it1, *it2);
            it2++;
        }
    }
    return ret;
}

void Constraint::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList) {
    std::vector<Expression_r> normalized;
    for (Expression &e : mExpressions) {
        normalized.push_back(e.normalize(constraintList, variableList));
    }
    constraintList.insert(build(normalized));
}

bool operator< (const Constraint &lhs, const Constraint &rhs) {
//    return (&lhs < &rhs);
    return &lhs!=&rhs && (typeid(lhs).before(typeid(rhs)) || (typeid(lhs)==typeid(rhs) && lhs.mExpressionSetId < rhs.mExpressionSetId));
//    return (&lhs != &rhs) && ((typeid(lhs).before(typeid(rhs))) || (typeid(lhs) == typeid(rhs) && lhs.mExpressions < rhs.mExpressions));
}
bool operator== (const Constraint &lhs, const Constraint &rhs) {
//    return (&lhs == &rhs);
    return (&lhs == &rhs) || (typeid(lhs)==typeid(rhs) && lhs.mExpressionSetId==rhs.mExpressionSetId);
//    return (&lhs == &rhs) || (typeid(lhs) == typeid(rhs) && lhs.mExpressions == rhs.mExpressions);
}

Constraint& Constraint::build(std::vector<Expression_r> &expressions) {
    throw std::logic_error(std::string(__FUNCTION__) + " was not implemented\n");
}

