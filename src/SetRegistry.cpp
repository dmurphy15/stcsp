#include "../include/SetRegistry.h"

#include "../include/Expression.h"

#include "../include/constraints/specialConstraints/PrimitiveUntilConstraint.h"

int SetRegistry::constraintSetIdSource = 0;
int SetRegistry::expressionSetIdSource = 0;
std::map<std::set<Constraint_r>, int> SetRegistry::constraintSetIds = {};
std::map<std::vector<Expression_r >, int> SetRegistry::expressionSetIds = {};

std::map<int, bool> SetRegistry::terminalConstraintSets = {};

bool SetRegistry::IsTerminalConstraintSet(int constraintSetId, const std::set<Constraint_r>& constraintSet) {
    if (terminalConstraintSets.find(constraintSetId) == terminalConstraintSets.end()) {
        bool isTerminal = true;
        for (Constraint &c : constraintSet) {
            if (typeid(c) == typeid(PrimitiveUntilConstraint)) {
                isTerminal = false;
                break;
            }
        }
        terminalConstraintSets[constraintSetId] = isTerminal;
    }
    return terminalConstraintSets.at(constraintSetId);
}

int SetRegistry::GetConstraintSetId(const std::set<Constraint_r>& constraintSet) {
    return getId<std::set<Constraint_r>>(constraintSetIds, constraintSetIdSource, constraintSet);
}

int SetRegistry::GetExpressionSetId(const std::vector<Expression_r>& expressionSet){
    return getId<std::vector<Expression_r>>(expressionSetIds, expressionSetIdSource, expressionSet);
}

template <class T>
int SetRegistry::getId(std::map<T, int>& map, int& idSource, const T& item) {
    auto it = map.find(item);
    if (it != map.end()) {
        return it->second;
    } else {
        int id = idSource++;
        map[item] = id;
        return id;
    }
}