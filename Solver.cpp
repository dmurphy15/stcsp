#include "Solver.h"

#include "constraints/PrimitiveNextConstraint.h"
#include "constraints/PrimitiveUntilConstraint.h"
#include "InstantSolverFactory.h"

Solver::Solver(std::set<Constraint_r> constraints, std::string instantSolverType) {
    std::set<Constraint_r> initialConstraints;
    for (Constraint &c : constraints) {
        std::set<Variable_r> vars = c.getVariables();
        mVariables.insert(mVariables.end(), vars.begin(), vars.end());
        mOriginalVariables.insert(mVariables.end(), vars.begin(), vars.end());
        // this will add normalized constraints that are equivalent to c to mConstraints, and it
        // will add any new variables that go with them to mVariables
        c.normalize(initialConstraints, mVariables);
    }
    mSolverType = instantSolverType;
    // allocates the new solver
    mStateTree = InstantSolverFactory.MakeInstantSolver(initialConstraints, mSolverType);
}

void Solver::solve() {
    solveRe(mStateTree);
}

bool Solver::solveRe(InstantSolver &currentState) {
    int numChildNodes = 0;
    // after normalizing everything, the nice thing is that we'll have created pretty much
    // unconstrained variables for "next" expressions (unconstrained during the current timepoint)

    // depending on the prefix-k we choose this might have to change to respect that
    for (auto &state : currentState.generateNextStatesIterator()) {
        std::map<Variable_r, int> carriedAssignments;
        std::set<Constraint_r> carriedConstraints = mCurrentState.getConstraints();
        std::tie(carriedConstraints, carriedAssignments) = carryConstraints(state);
        InstantSolver &nextState = InstantSolverFactory.MakeInstantSolver(carriedConstraints, mSolverType);
        InstantSolver &dominator = detectDominance(mStateTree, nextState);

        // dominating node found; add it as a child but no need to recur
        if (&dominator != &nextState) {
            currentState.addChildNode(dominator);
            numChildNodes++;
        // no dominating node found; recur on next state, but if next state is not successful, don't add it as a child
        } else {
            bool nextWasSuccessful = solveRe(nextState);
            if (nextWasSuccessful) {
                currentState.addChildNode(dominator);
                numChildNodes++;
            }
        }
    }
    // if we have no children, the current state has failed
    return numChildNodes > 0;
}

std::pair<std::set<Constraint_r>, std::map<Variable_r, int>> Solver::carryConstraints(std::map<Variable_r, int> assignment) {
    std::map<Variable_r, int> carriedAssignments;
    std::set<Constraint_r> carriedConstraints = mCurrentState.getConstraints();
    for (Constraint &c : carriedConstraints) {
        if (typeid(c) == typeid(PrimitiveNextConstraint)) {
           PrimitiveNextConstraint &c = static_cast<PrimitiveNextConstraint>(c);
           carriedAssignments[c.mNextVariable] = assignment[c.mVariable];
        } else if (typeid(c) == typeid(PrimitiveUntilConstraint)) {
            PrimitiveUntilConstraint &c = static_cast<PrimitiveUntilConstraint>(c);
            if (assignment[c.mUntilVariable] != 0) {
                carriedConstraints.erase(c);
            }
        }
    }
    return std::make_pair(carriedConstraints, carriedAssignments);
}


boost::optional<InstantSolver> Solver::detectDominance(InstantSolver &dominator, InstantSolver &dominated) {
    if (dominator == dominated) {
        return dominator;
    } else {
        for (InstantSolver &s : dominator.getChildNodes()) {
            InstantSolver &s2 = detectDominance(s, dominated);
            if (&s2 != &dominated) {
                return s2;
            }
        }
        return dominated;
    }
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
