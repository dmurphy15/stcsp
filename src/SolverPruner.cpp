#include "../include/SolverPruner.h"

#include <set>
#include <iostream>

#include "../include/SearchNode.h"
#include "../include/Constraint.h"
#include "../include/constraints/specialConstraints/PrimitiveUntilConstraint.h"

#include "../include/SetRegistry.h"

void SolverPruner::pruneForUntilConstraint(SearchNode& rootNode) {
    std::set<SearchNode *> terminalRoots = {}, terminalParents = {}, visited1 = {}, visited2 = {};
    findTerminalRoots(&rootNode, terminalRoots, visited1);
    for (SearchNode *node : terminalRoots) {
        gatherTerminalParents(node, terminalParents);
    }
    // if the root doesn't even lead to any terminal nodes, we
    // also need to cut its children out of the graph, since pruneForUntilConstraintRe
    // only cuts out parents
    if (terminalParents.find(&rootNode) == terminalParents.end()) {
        for (auto& pair : rootNode.getChildNodes()) {
            rootNode.removeChildNode(pair.first);
        }
    }
    pruneForUntilConstraintRe(&rootNode, terminalParents, terminalRoots, visited2);
}

/**
 * I'm saying a "terminal root" is a node with no until constraints, whose parent had until constraints
 * all children of a terminal root will also be terminal, so no need to consider pruning them. We only
 * need to prune nodes that are not parents of terminal roots.
 */
void SolverPruner::findTerminalRoots(SearchNode* currNode,
                               std::set<SearchNode *>& terminalRoots,
                               std::set<SearchNode *>& visited) {
    visited.insert(currNode);
    bool isTerminal = SetRegistry::IsTerminalConstraintSet(currNode->getConstraintSetId());
    if (isTerminal) {
        terminalRoots.insert(currNode);
        return;
    }
    for (auto pair : currNode->getChildNodes()) {
        SearchNode* n = pair.first;
        if (visited.find(n) == visited.end()) {
            findTerminalRoots(n, terminalRoots, visited);
        }
    }
}

/**
 * terminal parents are terminal, but they may have children that are not terminal
 */
void SolverPruner::gatherTerminalParents(SearchNode* currNode, std::set<SearchNode *>& terminalParents) {
    terminalParents.insert(currNode);
    for (SearchNode* parentNode : currNode->getParentNodes()) {
        if (terminalParents.find(parentNode) == terminalParents.end()) {
            gatherTerminalParents(parentNode, terminalParents);
        }
    }
}

/**
 * prune any nodes that are not terminal
 */
void SolverPruner::pruneForUntilConstraintRe(SearchNode* currNode,
                                       std::set<SearchNode *> &terminalParents,
                                       std::set<SearchNode *> &terminalRoots,
                                       std::set<SearchNode *> &visited) {
    visited.insert(currNode);
    // we are a terminal root; all children are terminal
    if (terminalRoots.find(currNode) != terminalRoots.end()) {
        return;
    }
    // we are a terminal parent that is not a terminal root; we are terminal, but must still check our children
    if (terminalParents.find(currNode) != terminalParents.end()) {
        for (auto pair : currNode->getChildNodes()) {
            SearchNode* child = pair.first;
            if (visited.find(child) == visited.end()) {
                pruneForUntilConstraintRe(child, terminalParents, terminalRoots, visited);
            }
        }
        return;
    }
    // this node is not terminal; cut it out of the graph
    //TODO: MEMORY LEAKS!!!!!
    for (SearchNode* parent : currNode->getParentNodes()) {
        // for completeness, I could clear the list of parentNodes contained in currNode, but
        // that would just waste time
        parent->removeChildNode(currNode);

    }
}