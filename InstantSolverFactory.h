#pragma once

#include <string>
#include <functional>
#include <stdexcept>

#include "InstantSolver.h"
#include "instantSolvers/NullInstantSolver.h"
#include "instantSolvers/GACInstantSolver.h"

enum InstantSolverType : int {
    NULL_INSTANT_SOLVER,
    GAC_INSTANT_SOLVER
};

class InstantSolverFactory
{
    using Variable_r = std::reference_wrapper<Variable>;
    using Constraint_r = std::reference_wrapper<Constraint>;
public:
    InstantSolverFactory() = delete;
    static InstantSolver& MakeInstantSolver(InstantSolverType instantSolverType,
                                           std::set<Constraint_r> constraints,
                                            std::map<Variable_r, int> inputAssignments) {
        switch (instantSolverType) {
            case NULL_INSTANT_SOLVER :
                return *new NullInstantSolver({}, {});
            case GAC_INSTANT_SOLVER :
                return *new GACInstantSolver(constraints, inputAssignments);
            default :
                throw std::invalid_argument("use a valid instant solver name pls. thx.\n");
        }
    }
};