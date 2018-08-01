#include "Solver.h"

#include <iostream>

#include "constraints/PrimitiveNextConstraint.h"
#include "constraints/PrimitiveUntilConstraint.h"

#include "InstantSolverFactory.h"

#include "Variable.h"
#include "Constraint.h"

Solver::Solver(std::set<Constraint_r> constraints, InstantSolverType instantSolverType) {
    std::set<Constraint_r> initialConstraints;
    for (Constraint &c : constraints) {
        std::set<Variable_r> vars = c.getVariables();
        mVariables.insert(vars.begin(), vars.end());
        mOriginalVariables.insert(vars.begin(), vars.end());
        // this will add normalized constraints that are equivalent to c to mConstraints, and it
        // will add any new variables that go with them to mVariables
        c.normalize(initialConstraints, mVariables);
    }
    mSolverType = instantSolverType;
    // allocates the new solver
    mStateTree.reset(&InstantSolverFactory::MakeInstantSolver(mSolverType, initialConstraints, {}));
}

void Solver::solve() {
    solveRe(*mStateTree);
}

bool Solver::solveRe(InstantSolver &currentState) {
    int numChildNodes = 0;
    // after normalizing everything, the nice thing is that we'll have created pretty much
    // unconstrained variables for "next" expressions (unconstrained during the current timepoint)

    // depending on the prefix-k we choose this might have to change to respect that
    for (auto &state : currentState.generateNextStatesIterator()) {
        std::map<Variable_r, int> carriedAssignments;
        std::set<Constraint_r> carriedConstraints;
        std::tie(carriedConstraints, carriedAssignments) = carryConstraints(currentState.getConstraints(), state);
        InstantSolver &nextState = InstantSolverFactory::MakeInstantSolver(mSolverType, carriedConstraints, carriedAssignments);
        InstantSolver &dominator = detectDominance(*mStateTree, nextState, {});

        currentState.addChildNode(dominator, state);
        numChildNodes++;
        // if no dominating node was found, detectDominance will return nextState, and we will have added it as a
        // new child. We must now test the new child node for failure and remove it if it fails
        if (&dominator == &nextState) {
            bool nextWasSuccessful = solveRe(nextState);
            if (!nextWasSuccessful) {
                numChildNodes--;
                currentState.removeLastChildNode();
            }
        }
    }
    // if we have no children, the current state has failed
    return numChildNodes > 0;
}

std::pair<std::set<Constraint_r>, std::map<Variable_r, int>> Solver::carryConstraints(std::set<Constraint_r> constraints,
                                                                                      std::map<Variable_r, int> assignment) {
    std::map<Variable_r, int> carriedAssignments;
    std::set<Constraint_r> carriedConstraints = constraints;
    for (Constraint &c : carriedConstraints) {
        if (typeid(c) == typeid(PrimitiveNextConstraint)) {
           PrimitiveNextConstraint &pc = static_cast<PrimitiveNextConstraint &>(c);
           carriedAssignments[pc.mNextVariable] = assignment[pc.mVariable];
        } else if (typeid(c) == typeid(PrimitiveUntilConstraint)) {
            PrimitiveUntilConstraint &pc = static_cast<PrimitiveUntilConstraint &>(c);
            if (assignment[pc.mUntilVariable] != 0) {
                carriedConstraints.erase(c);
            }
        }
    }
    return std::make_pair(carriedConstraints, carriedAssignments);
}


InstantSolver& Solver::detectDominance(InstantSolver &dominator, InstantSolver &dominated, std::set<InstantSolver *> visited) {
    // we have visited this node before (due to a cycle) so break it
    if (visited.find(&dominator) != visited.end()) {
        return dominated;
    // since dominated has not yet been added to the tree, we have found a true valid dominator
    } else if (dominator == dominated) {
        return dominator;
    } else {
        visited.insert(&dominator);
        for (auto &pair : dominator.getChildNodes()) {
            InstantSolver &s = pair.first;
            InstantSolver &s2 = detectDominance(s, dominated, visited);
            if (&s2 != &dominated) {
                return s2;
            }
        }
        return dominated;
    }
}

void Solver::printTree(bool includeAuxiliaryVariables) {
    printTreeRe(*mStateTree, {}, includeAuxiliaryVariables);
    std::cout<<"\n";
}

std::set<InstantSolver *> Solver::printTreeRe(InstantSolver &currentState, std::set<InstantSolver *> visited, bool includeAuxiliaryVariables) {
    if (visited.find(&currentState) != visited.end()) {
        return visited;
    }
    std::cout<<"state at "<<&currentState<<":\n";
    for (auto &pair : currentState.getChildNodes()) {
        InstantSolver &child = pair.first;
        std::cout<<"\tchild at "<<&child<<" with assignments:\n";
        for (auto &assignment : pair.second) {
            Variable &v = assignment.first;
            if (mOriginalVariables.find(v) != mOriginalVariables.end() || includeAuxiliaryVariables) {
                std::cout<<"\t\tvariable at "<<&v<<" with value "<<assignment.second<<"\n";
            }
        }
    }
    visited.insert(&currentState);
    for (auto &pair : currentState.getChildNodes()) {
        InstantSolver &child = pair.first;
        visited = printTreeRe(child, visited, includeAuxiliaryVariables);
    }
    return visited;
}
//
//add variable
//    set prefixk for variable
//add constraint
//has variable-chooser protocol that it gives to each statenode
//also gives a value-chooser protocol to each statenode, but user can give each variable its own value-chooser as well
//also give constraint-chooser? maybe
//
//
//solve
//    create StateNode
//    iterate calling getnextassignment or something which will invoke its coroutine
//    if getnextassignment returns something, check for dominance, and place it further down the tree if not dominated
//
//
//StateNode:
//    has timeStep
//    has ordered set of constraints
//    has ordered set of variables
//    is give variable and value choosers
//    calls propagate on each variable iterating over values
//        and co_yield each total assignment when it finds one
//        can implement as a generator coroutine as described here kind of https://kirit.com/How%20C%2B%2B%20coroutines%20work/Generating%20Iterators
//
//
//prefix k is just how many steps previously we should look to make sure we're consistent with those previous steps
//normalization kind of makes this pointless since it reduces everything to just one 'next' at a time
//
//ASK JASPER IF THE PREFIX K SHOULD BE USED FOR NORMALIZATION, OR WHAT
//
//
//TAKE OUT UNORDERED SET AND REPLACE WITH AN ORDERED SET SINCE WE'LL BE ITERATING
//
