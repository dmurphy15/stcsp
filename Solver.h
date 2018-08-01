#pragma once

#include <vector>
#include <set>
#include <map>
#include <functional>
#include <memory>

#include "InstantSolverFactory.h"


// TODO GIVE THE SOLVER A PREFIX-K TO WORK WITH, WHICH WILL ALSO AFFECT HOW THE CONSTRAINTS NORMALIZE THEMSELVES, I BELIEVE (AT LEAST WRT NEXT EXPRESSIONS)
// TODO also need to give some constraints some notion of tautology for dominance detection

// TODO also if an until constraint is satisfied but the second variable is never > 0, then that's really a failure so we should check that at the end and fail



class Constraint;
class Variable;
class InstantSolver;










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
    using Variable_r = std::reference_wrapper<Variable>;
    using Constraint_r = std::reference_wrapper<Constraint>;
public:
    Solver(std::set<Constraint_r> constraints, InstantSolverType instantSolverType);
    void solve();

    InstantSolverType mSolverType;
    std::shared_ptr<InstantSolver> mStateTree;

    void printTree(bool includeAuxiliaryVariables=false);
private:
    // returns false if currentState is a failure node
    bool solveRe(InstantSolver &currentState);
    // creates the appropriate instantaneous assignments and constraints for the next state
    std::pair<std::set<Constraint_r>, std::map<Variable_r, int>> carryConstraints(std::set<Constraint_r> constraints,
                                                                                  std::map<Variable_r, int> assignment);
    // returns dominated if no nodes in dominator tree were dominating
    InstantSolver& detectDominance(InstantSolver &dominator, InstantSolver &dominated, std::set<InstantSolver *> visited);

    std::set<InstantSolver *> printTreeRe(InstantSolver &currentState, std::set<InstantSolver *> visited, bool includeAuxiliaryVariables);

//    std::set<Constraint_r> mConstraints;
    std::set<Variable_r> mVariables;
    std::set<Variable_r> mOriginalVariables;
};