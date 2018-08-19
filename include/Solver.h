#pragma once

#include <vector>
#include <set>
#include <map>
#include <functional>
#include <memory>

#include "types.h"
#include "SearchNodeFactory.h"

// TODO GIVE THE SOLVER A PREFIX-K TO WORK WITH, WHICH WILL ALSO AFFECT HOW THE CONSTRAINTS NORMALIZE THEMSELVES, I BELIEVE (AT LEAST WRT NEXT EXPRESSIONS)
// TODO also need to give some constraints some notion of tautology for dominance detection

// TODO also if an until constraint is satisfied but the second variable is never > 0, then that's really a failure so we should check that at the end and fail


//WHY DO WE NEED THIS NORMALIZATION TO PRIMITIVE UNTIL CONSTRAINTS ACTUALLY??????????????????????????????????
//
//I SEE THAT WE NEED TO NORMALIZE NEXT EXPRESSIONS SO THAT WE CAN CREATE AN UNCONSTRAINED VARIABLE AT THE
//CURRENT TIME POINT AND USE ITS VALUE TO CONSTRAIN THE NEXT TIME POINT SINCE WE CANNOT EVALUATE A NEXT
//EXPRESSION DIRECTLY WITHIN ONE TIMEPOINT
//
//BUT AS LONG AS THE UNTIL CONSTRAINT DOES NOT CONTAIN NEXT EXPRESSIONS (AND IT WONT SINCE WE NORMALIZE THEM)
//WE CAN EVALUATE IT WITHIN A SINGLE TIMEPOINT RIGHT? IS CAN BE EVALUATED WITHIN A SINGLE TIMEPOINT, EVEN IF IT OPERATES ON
//EXPRESSIONS RATHER THAN SINGLE VARIABLES. WE REMOVE IT ALL THE SAME IF THE "UNTIL" VARIABLE (OR EXPRESSION)
//IS EVER 1




















class Solver
{
public:
    Solver(SearchNodeType searchNodeType, int prefixK);
    Solver(SearchNodeType searchNodeType, int prefixK, std::set<Constraint_r> constraints);
    void addConstraint(Constraint &c);
    void solve();
    void printTree(bool includeAuxiliaryVariables=false);
private:
    SearchNodeType mNodeType;
    std::shared_ptr<SearchNode> mTree;

    // returns false if currentState is a failure node
    bool solveRe(SearchNode &currentNode);
    // creates the appropriate instantaneous assignments and constraints for the next state
    std::pair<std::set<Constraint_r>, assignment_t> carryConstraints(std::set<Constraint_r> constraints,
                                                                      assignment_t assignment);
    // returns dominated if no nodes in dominator tree were dominating
    SearchNode& detectDominance(SearchNode &newNode);

    std::set<SearchNode *>& printTreeRe(SearchNode &currentState, std::set<SearchNode *> &visited, bool includeAuxiliaryVariables);

    std::set<Variable_r> mOriginalVariables;
    std::set<Constraint_r> mOriginalConstraints;
    // just for documentation; unneeded
    std::set<Variable_r> mVariables;


    //TODO in the future it would be pretty easy to also keep a set of seen failure nodes, so we can cut short any nodes
    // that we know will fail
    std::set<SearchNode_r> mSeenSearchNodes;

    std::map<Variable_r, domain_t> mDomainsInitializer;

    int mPrefixK;
};