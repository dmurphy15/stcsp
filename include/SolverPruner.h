#pragma once

#include <set>

class SearchNode;

/***************************** SOLVERPRUNER ****************************************************************************
 * Takes a solution graph and prunes searchnodes that are not terminal nodes
 * A searchnode is not terminal if there is an UntilConstraint that can never be satisfied by any future assignments
 * that stem from this node
 * ********************************************************************************************************************/

class SolverPruner
{
public:
    SolverPruner() = delete;
    /**
     * take the solution graph and prune off any nodes that are not terminal nodes
     * @param rootNode - the root node of the solution graph
     */
    static void pruneForUntilConstraint(SearchNode& rootNode);

private:
    /**
     * Gather all nodes where an UntilConstraint was not satisfied by at least one parent, but where
     * all UntilConstraints are now satisfied (calling it a terminal root)
     * @param currNode - current node we are looking at
     * @param terminalRoots - set of nodes we are gathering
     * @param visited - set of visited nodes (to prevent looping)
     */
    static void findTerminalRoots(SearchNode* currNode,
                           std::set<SearchNode *>& terminalRoots,
                           std::set<SearchNode *>& visited);
    /**
     * Gather all ancestor nodes of terminal roots (calling them terminal parents)
     * @param currNode - current node we are looking at
     * @param terminalParents - set in which to gather the nodes
     */
    static void gatherTerminalParents(SearchNode* currNode, std::set<SearchNode *>& terminalParents);
    /**
     * helper to prune all nodes that are non-terminal
     * @param currNode - current node we are looking at
     * @param terminalParents - set of terminal parents
     * @param terminalRoots - set of terminal roots
     * @param visited - set of visited nodes (to prevent looping)
     */
    static void pruneForUntilConstraintRe(SearchNode* currNode,
                                   std::set<SearchNode *> &terminalParents,
                                   std::set<SearchNode *> &terminalRoots,
                                   std::set<SearchNode *> &visited);
};