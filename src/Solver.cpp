#include "../include/Solver.h"

#include <algorithm>

#include "../include/constraints/specialConstraints/PrimitiveAtConstraint.h"
#include "../include/constraints/specialConstraints/PrimitiveNextConstraint.h"
#include "../include/constraints/specialConstraints/PrimitiveUntilConstraint.h"
#include "../include/constraints/specialConstraints/PrimitiveFirstConstraint.h"
#include "../include/constraints/specialConstraints/EqualConstraint.h"

#include "../include/expressions/specialExpressions/ConstantExpression.h"

#include "../include/SearchNode.h"
#include "../include/SearchNodeFactory.h"

#include "../include/SetRegistry.h"

#include "../include/Variable.h"
#include "../include/Constraint.h"

#include "../include/SolverPruner.h"
#include "../include/SolverPrinter.h"

Solver::Solver(SearchNodeType searchNodeType, int prefixK) {
    mNodeType = searchNodeType;
    mPrefixK = prefixK;
}

Solver::Solver(SearchNodeType searchNodeType, int prefixK, const std::set<Constraint_r>& constraints) {
    mNodeType = searchNodeType;
    mPrefixK = prefixK;
    mOriginalConstraints = constraints;
    for (Constraint &c: constraints) {
        std::set<Variable_r>&& vars = c.getVariables(true);
        mOriginalVariables.insert(vars.begin(), vars.end()); // note that this means that only variables that were constrained will appear in the solution
    }
}

void Solver::addConstraint(Constraint &c) {
    mOriginalConstraints.insert(c);
    std::set<Variable_r>&& vars = c.getVariables(true);
    mOriginalVariables.insert(vars.begin(), vars.end());
}

void Solver::solve() {
    mDomainsInitializer.clear();
    mSeenSearchNodes.clear();
    mVariables.clear();

    std::set<Constraint_r> initialConstraints;
    mVariables = mOriginalVariables;
    for (Constraint &c : mOriginalConstraints) {
        // this will add normalized constraints that are equivalent to c to mConstraints, and it
        // will add any new variables that go with them to mVariables
        c.normalize(initialConstraints, mVariables);
    }
    for (Variable& v: mVariables) {
        mDomainsInitializer.insert({v, v.getInitialDomain()});
    };
    std::vector<std::map<Variable_r, domain_t>> initialDomains;
    for (int i=0; i < mPrefixK; i++) {
        initialDomains.push_back(mDomainsInitializer);
    }
    // allocates the new solver
    assignment_t initialAssignments;
    mTree.reset(&SearchNodeFactory::MakeSearchNode(mNodeType, initialConstraints, initialAssignments, initialDomains));
    solveRe(*mTree);
    SolverPruner::pruneForUntilConstraint(*mTree);
}

bool Solver::solveRe(SearchNode &currentNode) {
    mSeenSearchNodes[currentNode] = true;
    int numChildNodes = 0;
    for (assignment_t& assignment : currentNode.generateNextAssignmentIterator()) {
        if (&currentNode == SearchNode::root) {
            currentNode.setAssignments(assignment, 0);
        }
        std::set<Constraint_r> carriedConstraints; assignment_t carriedAssignments;
        bool changedConstraintSet = carryConstraints(currentNode.getConstraints(),
                                                      assignment,
                                                      carriedConstraints,
                                                      carriedAssignments,
                                                      &currentNode==SearchNode::root);
        int nextConstraintSetId;
        if (changedConstraintSet) {
            nextConstraintSetId = SetRegistry::GetConstraintSetId(carriedConstraints);
        } else {
            nextConstraintSetId = currentNode.getConstraintSetId();
        }
        std::vector<std::map<Variable_r, domain_t>> nextInitialDomains(mPrefixK);
        for (int i=0; i < mPrefixK - 1; i++) {
            nextInitialDomains[i] = currentNode.getDomains(i+1);
        }
        nextInitialDomains[mPrefixK-1] = mDomainsInitializer;
        SearchNode &nextNode = SearchNodeFactory::MakeSearchNode(mNodeType,
                                                                 carriedConstraints,
                                                                 carriedAssignments,
                                                                 nextInitialDomains,
                                                                 nextConstraintSetId);

        // detect dominance
        auto dominator = mSeenSearchNodes.find(nextNode);
        SearchNode &child = (dominator == mSeenSearchNodes.end()) ? nextNode : dominator->first.get();

        bool notSeenFailed = &child == &nextNode || dominator->second;
        // if the next node is not necessarily failure, look into it; else we can just move on
        if (notSeenFailed) {
            currentNode.addChildNode(&child, assignment);
            child.addParentNode(&currentNode);
            numChildNodes++;
            // if nextNode has never been seen before, we must go forward to make sure it doesn't fail
            if (&child == &nextNode) {
                bool nextWasSuccessful = solveRe(nextNode);
                if (!nextWasSuccessful) {
                    numChildNodes--;
                    currentNode.removeChildNode(&child);
                    nextNode.removeParentNode(&currentNode);
                    mSeenSearchNodes[nextNode] = false; // mark it as failed
                }
            }
        }
    }
    // if we have no children, the current state has failed
    return numChildNodes > 0;
}

bool Solver::carryConstraints(const std::set<Constraint_r>& constraints,
                         const assignment_t& assignment,
                         std::set<Constraint_r>& carriedConstraints,
                         assignment_t& carriedAssignments,
                              bool solvingFirstNode) {
    bool changedConstraintSet = false;
    std::set<Constraint_r> added = {};
    if (solvingFirstNode) {
        carriedConstraints = freezeFirstExpressions(constraints);
        changedConstraintSet = true;
    } else {
        carriedConstraints = constraints;
    }
    for (auto it = carriedConstraints.begin(); it != carriedConstraints.end();) {
        Constraint &c = *it;
        //TODO whenever I erase a constraint below, I erase references to expressions and probably cause memory leaks
        bool inc = true;
        if (solvingFirstNode) {
            std::set<Variable_r> vs = c.getVariables(false); // setting root=false since now we're done solving the first node
            if (vs.size() == 0) { // it is a tautology if everything it deals with is constant. had to do some trickery with first expressions to do this
                it = carriedConstraints.erase(it);
                inc = false;
                changedConstraintSet = true;
                continue;
            }
        }
        if (typeid(c) == typeid(PrimitiveFirstConstraint)) {
            it = carriedConstraints.erase(it);
            inc = false;
            changedConstraintSet = true;
        } else if (typeid(c) == typeid(PrimitiveNextConstraint)) {
            PrimitiveNextConstraint &pc = static_cast<PrimitiveNextConstraint &>(c);
            carriedAssignments[pc.mNextVariable] = assignment.at(pc.mVariable);
        } else if (typeid(c) == typeid(PrimitiveUntilConstraint)) {
            PrimitiveUntilConstraint &pc = static_cast<PrimitiveUntilConstraint &>(c);
            if (assignment.at(pc.mUntilVariable) != 0) {
                it = carriedConstraints.erase(it);
                inc = false;
                changedConstraintSet = true;
            }
        } else if (typeid(c) == typeid(PrimitiveAtConstraint)) {
            PrimitiveAtConstraint &pc = static_cast<PrimitiveAtConstraint &>(c);
            added.insert(pc.makeDecrementedCopy());
            it = carriedConstraints.erase(it);
            inc = false;
            changedConstraintSet = true;
        }

        if (inc) { ++it; }
    }
    carriedConstraints.insert(added.begin(), added.end());
    return changedConstraintSet;
}

// we should only ever have to freeze these a few times, after solving root, so even though we have
// to traverse each constraint tree hopefully it won't take too long
std::set<Constraint_r> Solver::freezeFirstExpressions(const std::set<Constraint_r>& constraints) {
    std::set<Constraint_r> ret = {};
    for (Constraint& c : constraints) {
        ret.insert(c.freezeFirstExpressions());
    }
    return ret;
}

void Solver::printTree() { SolverPrinter::printTree(*this); }
void Solver::writeGraph() { SolverPrinter::writeGraph(*this); }
