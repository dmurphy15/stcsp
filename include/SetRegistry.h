#pragma once
#include "types.h"

class SetRegistry
{
public:
    static int GetConstraintSetId(std::set<Constraint_r> constraintSet);
    static int GetExpressionSetId(std::vector<Expression_r> expressionSet);

    static bool IsTerminalConstraintSet(int constraintSetId);
private:
    template <class T>
    static int getId(std::map<T, int>& map, int& idSource, T& set);

    static std::map<std::set<Constraint_r>, int> constraintSetIds;
    static std::map<std::vector<Expression_r>, int> expressionSetIds;

    static int constraintSetIdSource;
    static int expressionSetIdSource;

    // maps a constraintSetId to a bool indicating whether or not that
    // constraint set contains no PrimitiveUntilConstraints (if it contains
    // PrimitiveUntilConstraints then the assignment leading to this SearchNode
    // was not completely satisfactory yet)
    static std::map<int, bool> terminalConstraintSets;
};