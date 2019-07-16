#include "../include/SetRegistry.h"

#include "../include/Expression.h"

#include "../include/constraints/specialConstraints/PrimitiveUntilConstraint.h"

int SetRegistry::constraintSetIdSource = 0;
int SetRegistry::expressionSetIdSource = 0;
std::map<std::set<Constraint_r>, int> SetRegistry::constraintSetIds = {};
std::map<std::vector<Expression_r >, int> SetRegistry::expressionSetIds = {};

std::map<int, bool> SetRegistry::terminalConstraintSets = {};

bool SetRegistry::IsTerminalConstraintSet(int constraintSetId) {
    return terminalConstraintSets.at(constraintSetId);
}

int SetRegistry::GetConstraintSetId(std::set<Constraint_r> constraintSet) {
    int id = getId<std::set<Constraint_r>>(constraintSetIds, constraintSetIdSource, constraintSet);

    if (terminalConstraintSets.find(id) == terminalConstraintSets.end()) {
        bool isTerminal = true;
        for (Constraint &c : constraintSet) {
            if (typeid(c) == typeid(PrimitiveUntilConstraint)) {
                isTerminal = false;
                break;
            }
        }
        terminalConstraintSets[id] = isTerminal;
    }

    return id;
}

int SetRegistry::GetExpressionSetId(std::vector<Expression_r> expressionSet){
    return getId<std::vector<Expression_r>>(expressionSetIds, expressionSetIdSource, expressionSet);
}

template <class T>
int SetRegistry::getId(std::map<T, int>& map, int& idSource, T& item) {
    auto it = map.find(item);
    if (it != map.end()) {
        return it->second;
    } else {
        int id = idSource++;
        map[item] = id;
        return id;
    }
}