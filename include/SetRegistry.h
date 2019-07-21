#pragma once
#include "types.h"

/***************************** SETREGISTRY *****************************************************************************
 * Allows us to compare SearchNodes and Constraints a bit more efficiently, by providing a unique id
 * for each unique set of constraints in a SearchNode or vector of Expressions in a Constraint.
 * Also keeps track of which constraint sets contain unsatisfied UntilConstraints, for the SolverPruner
 * ********************************************************************************************************************/

class SetRegistry
{
public:
    /**
     * Get a unique id for a set of Constraints/vector of Expressions
     */
    static int GetConstraintSetId(const std::set<Constraint_r>& constraintSet);
    static int GetExpressionSetId(const std::vector<Expression_r>& expressionSet);

    /**
     * Look up whether this constraint set corresponds to a terminal searchNode
     * @param constraintSetId - id of the constraint set
     * @param constraintSet - the actual constraint set
     * @return whether or not the constraint set contains no UntilConstraints
     */
    static bool IsTerminalConstraintSet(int constraintSetId, const std::set<Constraint_r>& constraintSet);
private:
    /**
     * helper for getting a unique id for a constraint/expression set
     * @tparam T - whether we're looking for a constraint set or expression vector
     * @param map - the map to look it up in
     * @param idSource - the source from which to generate a new id if needed
     * @param set - the set we are looking up
     * @return a unique id for the set
     */
    template <class T>
    static int getId(std::map<T, int>& map, int& idSource, const T& set);

    /** maps from constraint/expression sets we have seen to their unique ids */
    static std::map<std::set<Constraint_r>, int> constraintSetIds;
    static std::map<std::vector<Expression_r>, int> expressionSetIds;

    /** sources from which to generate new unique ids */
    static int constraintSetIdSource;
    static int expressionSetIdSource;

    /** maps a constraintSetId to a bool indicating whether or not that constraint set contains no
     * PrimitiveUntilConstraints (if it contains PrimitiveUntilConstraints then the assignment leading
     * to this SearchNode was not completely satisfactory yet) */
    static std::map<int, bool> terminalConstraintSets;
};