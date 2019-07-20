#pragma once

#include <set>

class SearchNode;

class SolverPruner
{
public:
    SolverPruner() = delete;

    static void pruneForUntilConstraint(SearchNode& rootNode);

private:
    static void findTerminalRoots(SearchNode* currNode,
                           std::set<SearchNode *>& terminalRoots,
                           std::set<SearchNode *>& visited);
    static void gatherTerminalParents(SearchNode* currNode, std::set<SearchNode *>& terminalParents);
    static void pruneForUntilConstraintRe(SearchNode* currNode,
                                   std::set<SearchNode *> &terminalParents,
                                   std::set<SearchNode *> &terminalRoots,
                                   std::set<SearchNode *> &visited);
};