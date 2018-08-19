#include "../include/Solver.h"

#include <iostream>
#include <algorithm>

#include "../include/specialConstraints/PrimitiveNextConstraint.h"
#include "../include/specialConstraints/PrimitiveUntilConstraint.h"
#include "../include/specialConstraints/PrimitiveFirstConstraint.h"
#include "../include/specialConstraints/EqualConstraint.h"

#include "../include/specialExpressions/ConstantExpression.h"

#include "../include/SearchNode.h"
#include "../include/SearchNodeFactory.h"

#include "../include/Variable.h"
#include "../include/Constraint.h"

Solver::Solver(SearchNodeType searchNodeType, int prefixK) {
    mNodeType = searchNodeType;
    mPrefixK = prefixK;
}

Solver::Solver(SearchNodeType searchNodeType, int prefixK, std::set<Constraint_r> constraints) {
    mNodeType = searchNodeType;
    mPrefixK = prefixK;
    mOriginalConstraints = constraints;
    for (Constraint &c: constraints) {
        std::set<Variable_r> vars = c.getVariables();
        mOriginalVariables.insert(vars.begin(), vars.end());
    }
}

void Solver::addConstraint(Constraint &c) {
    mOriginalConstraints.insert(c);
}

void Solver::solve() {
    mDomainsInitializer.clear();
    mSeenSearchNodes.clear();
    mVariables.clear();

    std::set<Constraint_r> initialConstraints;
    for (Constraint &c : mOriginalConstraints) {
        std::set<Variable_r> vars = c.getVariables();
        mVariables.insert(vars.begin(), vars.end());
        mOriginalVariables.insert(vars.begin(), vars.end());
        // this will add normalized constraints that are equivalent to c to mConstraints, and it
        // will add any new variables that go with them to mVariables
        c.normalize(initialConstraints, mVariables);
    }
    for (Variable& v: mVariables) {
        mDomainsInitializer.insert({v, v.getInitialDomain()});
    };
    std::vector<std::map<Variable_r, domain_t>> initialDomains; initialDomains.reserve(mPrefixK);
    for (int i=0; i < mPrefixK; i++) {
        initialDomains.push_back(mDomainsInitializer);
    }
    // allocates the new solver
    mTree.reset(&SearchNodeFactory::MakeSearchNode(mNodeType, initialConstraints, {}, initialDomains));
    solveRe(*mTree);
}

bool Solver::solveRe(SearchNode &currentNode) {
    mSeenSearchNodes.insert(currentNode);
    int numChildNodes = 0;
    // after normalizing everything, the nice thing is that we'll have created pretty much
    // unconstrained variables for "next" expressions (unconstrained during the current timepoint)

    // depending on the prefix-k we choose this might have to change to respect that
    for (assignment_t& assignment : currentNode.generateNextAssignmentIterator()) {
        assignment_t carriedAssignments;
        std::set<Constraint_r> carriedConstraints;
        std::tie(carriedConstraints, carriedAssignments) = carryConstraints(currentNode.getConstraints(), assignment);
        std::vector<std::map<Variable_r, domain_t>> nextInitialDomains(mPrefixK);
        for (int i=0; i < mPrefixK - 1; i++) {
            nextInitialDomains[i] = currentNode.getDomains(i+1);
        }
        nextInitialDomains[mPrefixK-1] = mDomainsInitializer;
        SearchNode &nextNode = SearchNodeFactory::MakeSearchNode(mNodeType, carriedConstraints, carriedAssignments, nextInitialDomains);
        // detect dominance
        auto dominator = mSeenSearchNodes.find(nextNode);
        SearchNode &child = (dominator == mSeenSearchNodes.end()) ? nextNode : (*dominator).get();

        currentNode.addChildNode(child, assignment);
        numChildNodes++;
        // if no dominating node was found, detectDominance will return nextState, and we will have added it as a
        // new child. We must now test the new child node for failure and remove it if it fails
        if (&child == &nextNode) {
            bool nextWasSuccessful = solveRe(nextNode);
            if (!nextWasSuccessful) {
                numChildNodes--;
                currentNode.removeLastChildNode();
                mSeenSearchNodes.erase(nextNode);
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
    for (Constraint &c : constraints) {
        // primitive first constraints force an auxiliary variable to equal another variable, but we only want this
        // enforced for the first timepoint, so remove them immediately, and then enforce that the auxiliary variable
        // remains constant by saying x = next x
        // note that I could have also just done x = constant, but using next(x) is faster since it
        // will fix the domain to a single value upon constructing each new search node, as opposed to requiring more propagation
        // ACTUALLY, I'm switching to setting it to a constant since PrimitiveNextConstraint assumes that you are not seeing
        // x = next x (otherwise propagating it would be more complicated)
        // ACTUALLY I think I could switch back to the original way if I wanted, since I fixed that issue I believe, and I think that would be faster, but for now I'm just leaving it to be safe...
        //TODO whenever I erase a constraint below, I erase references to expressions and probably cause memory leaks
        if (typeid(c) == typeid(PrimitiveFirstConstraint)) {
            carriedConstraints.erase(c);
            PrimitiveFirstConstraint &pc = static_cast<PrimitiveFirstConstraint &>(c);
            VariableExpression &ve = *new VariableExpression(pc.mVariable);
            carriedConstraints.insert(*new EqualConstraint(ve, *new ConstantExpression(assignment[pc.mVariable])));
        } else if (typeid(c) == typeid(PrimitiveNextConstraint)) {
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

void Solver::printTree(bool includeAuxiliaryVariables) {
    std::set<SearchNode *> visited;
    printTreeRe(*mTree, visited, includeAuxiliaryVariables);
    std::cout<<"\n";
}

std::set<SearchNode *>& Solver::printTreeRe(SearchNode &currentState, std::set<SearchNode *> &visited, bool includeAuxiliaryVariables) {
    if (visited.find(&currentState) != visited.end()) {
        return visited;
    }
    std::cout<<"state at "<<&currentState<<":\n";
    for (auto &pair : currentState.getChildNodes()) {
        SearchNode &child = pair.first;
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
        SearchNode &child = pair.first;
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
