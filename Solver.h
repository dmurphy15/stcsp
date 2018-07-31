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


class Solver
{
    using Variable_r = std::reference_wrapper<Variable>;
    using Constraint_r = std::reference_wrapper<Constraint>;
public:
    Solver(std::set<Constraint_r> constraints, InstantSolverType instantSolverType);
    void solve();

    InstantSolverType mSolverType;
    std::shared_ptr<InstantSolver> mStateTree;

    void printTree();
private:
    // returns false if currentState is a failure node
    bool solveRe(InstantSolver &currentState);
    // creates the appropriate instantaneous assignments and constraints for the next state
    std::pair<std::set<Constraint_r>, std::map<Variable_r, int>> carryConstraints(std::set<Constraint_r> constraints,
                                                                                  std::map<Variable_r, int> assignment);
    // returns dominated if no nodes in dominator tree were dominating
    InstantSolver& detectDominance(InstantSolver &dominator, InstantSolver &dominated, std::set<InstantSolver *> visited);

    void printTreeRe(InstantSolver &currentState, std::set<InstantSolver *> visited);

//    std::set<Constraint_r> mConstraints;
    std::set<Variable_r> mVariables;
    std::set<Variable_r> mOriginalVariables;
};