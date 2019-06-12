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


#include <iostream>

Solver::Solver(SearchNodeType searchNodeType, int prefixK) {
    mNodeType = searchNodeType;
    mPrefixK = prefixK;
}

Solver::Solver(SearchNodeType searchNodeType, int prefixK, const std::set<Constraint_r>& constraints) {
    mNodeType = searchNodeType;
    mPrefixK = prefixK;
    mOriginalConstraints = constraints;
    for (Constraint &c: constraints) {
        c.getVariables(mOriginalVariables);
    }
}

void Solver::addConstraint(Constraint &c) {
    mOriginalConstraints.insert(c);
    c.getVariables(mOriginalVariables);
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
    std::vector<std::pair<std::map<Variable_r, domain_t>::const_iterator, std::map<Variable_r, domain_t>::const_iterator>> initialDomains;
    for (int i=0; i < mPrefixK; i++) {
        initialDomains.push_back({mDomainsInitializer.begin(), mDomainsInitializer.end()});
    }
    // allocates the new solver
    assignment_t initialAssignments;
    mTree.reset(&SearchNodeFactory::MakeSearchNode(mNodeType, initialConstraints, initialAssignments, initialDomains));
    solveRe(*mTree);
    SolverPruner::pruneForUntilConstraint(*mTree);
}

bool Solver::solveRe(SearchNode &currentNode) {
    mSeenSearchNodes.insert(currentNode);
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
        std::vector<std::pair<std::map<Variable_r, domain_t>::const_iterator,
                              std::map<Variable_r, domain_t>::const_iterator>> nextInitialDomains(mPrefixK);
        for (int i=0; i < mPrefixK - 1; i++) {
            nextInitialDomains[i] = {currentNode.getDomains(i+1).begin(), currentNode.getDomains(i+1).end()};
        }
        nextInitialDomains[mPrefixK-1] = {mDomainsInitializer.begin(), mDomainsInitializer.end()};
        SearchNode &nextNode = SearchNodeFactory::MakeSearchNode(mNodeType,
                                                                 carriedConstraints,
                                                                 carriedAssignments,
                                                                 nextInitialDomains,
                                                                 nextConstraintSetId);
        // detect dominance
        auto dominator = mSeenSearchNodes.find(nextNode);
        SearchNode &child = (dominator == mSeenSearchNodes.end()) ? nextNode : (*dominator).get();

        currentNode.addChildNode(child, assignment);
        child.addParentNode(currentNode);
        numChildNodes++;

        // if nextNode has never been seen before, we must go forward to make sure it doesn't fail
        if (&child == &nextNode) {
            bool nextWasSuccessful = solveRe(nextNode);
            if (!nextWasSuccessful) {
                numChildNodes--;
                currentNode.removeLastChildNode();
                nextNode.removeParentNode(currentNode);
                mSeenSearchNodes.erase(nextNode);
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
    carriedConstraints = constraints;
    for (Constraint &c : constraints) {
        //TODO whenever I erase a constraint below, I erase references to expressions and probably cause memory leaks

        // tautology detection
//        if (solvingFirstNode) {
//            std::set<Variable_r> vs;
//            // c must have been satisfied. Also, constant expressions do not contain variables.
//            // Thus the constraint is a tautology if and only if there are no variables, or all variables have v == next v
//            c.getVariables(vs);
//            bool tautology = true;
//            for (auto v : vs) {
//                VariableExpression ve(v);
//                PrimitiveNextConstraint pc(ve, ve);
//                if (c == pc || constraints.find(pc) == constraints.end()) {
//                    tautology = false;
//                    break;
//                }
//            }
//            if (tautology) {
//                carriedConstraints.erase(c);
//                continue;
//            }
//        }

        // here's another attempt at tautology detection; doesn't mean the first attempt was wrong though
        if (solvingFirstNode) {
            std::set<Variable_r> vs;
            c.getVariables(vs, false); // setting root=false since now we're done solving the first node
            if (vs.size() == 0) { // it is a tautology if everything it deals with is constant. had to do some trickery with first expressions to do this
                carriedConstraints.erase(c);
                changedConstraintSet = true;
                continue;
            }
        }
        if (typeid(c) == typeid(PrimitiveFirstConstraint)) {
            carriedConstraints.erase(c);
            changedConstraintSet = true;
        } else if (typeid(c) == typeid(PrimitiveNextConstraint)) {
           PrimitiveNextConstraint &pc = static_cast<PrimitiveNextConstraint &>(c);
           carriedAssignments[pc.mNextVariable] = assignment.at(pc.mVariable);
        } else if (typeid(c) == typeid(PrimitiveUntilConstraint)) {
            PrimitiveUntilConstraint &pc = static_cast<PrimitiveUntilConstraint &>(c);
            if (assignment.at(pc.mUntilVariable) != 0) {
                carriedConstraints.erase(c);
                changedConstraintSet = true;
            }
        } else if (typeid(c) == typeid(PrimitiveAtConstraint)) {
            carriedConstraints.erase(c);
            PrimitiveAtConstraint &pc = static_cast<PrimitiveAtConstraint &>(c);
            carriedConstraints.insert(pc.makeDecrementedCopy());
            changedConstraintSet = true;
        }
    }
    return changedConstraintSet;
}

void Solver::printTree() { SolverPrinter::printTree(*this); }
void Solver::writeGraph() { SolverPrinter::writeGraph(*this); }
