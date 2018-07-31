#pragma once

#include <map>
#include <set>
#include <functional>
#include <vector>

#include "../InstantSolver.h"

class GACInstantSolver : public InstantSolver
{
public:
    GACInstantSolver(std::set<Constraint_r> constraints, std::map<Variable_r, int> inputAssignments={});
    std::vector<int> defaultPropagate(Variable &v, Constraint &c) override;
    coro_assignment_t::pull_type generateNextStatesIterator() override {
        return coro_assignment_t::pull_type(boost::bind(&GACInstantSolver::generateNextStates, this, boost::placeholders::_1));
    }

private:
    std::map<Variable_r, std::vector<Constraint_r>> mVariableToConstraints;
    std::map<Constraint_r, std::vector<Variable_r>> mConstraintToVariables;

    std::map<Variable_r, std::vector<int>> GAC();
    std::pair<std::vector<int>, std::vector<int>> splitDomain(std::vector<int> domain);
    void generateNextStates(coro_assignment_t::push_type &yield) override;
    void generateAssignments(coro_int_t::push_type &yield, std::vector<Variable_r> variables);
};

//REPLACE ALL UNORDERED_SETS with regular, ordered, sets, or at least think about it.
//One issue with sets of pointers is that it would compare identity rather than making sure the constriaints
//have the same values (are the same type and have the same member expressions). But if we are careful to
//never copy constraints after we begin solving, we may be alright
//
//future optimization - instead of iterating through all bottommost variables contained by the constraints
//directly, break them up so the expressions can create equivalent variables whose domains are sort of
//arrays whose initial elements represent combinations of values of the input variables and whose last
//element represents the expression's evaluated value - memory inefficient
//could use this for binarization to use AC-3 algorithm
