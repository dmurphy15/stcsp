#pragma once

#include <map>
#include <set>
#include <functional>
#include <vector>

#include "../SearchNode.h"

class GACSearchNode : public SearchNode
{
public:
    GACSearchNode(std::set<Constraint_r> constraints, assignment_t historicalValues, std::vector<std::map<Variable_r, domain_t>> domains);
    std::vector<std::set<int>> defaultPropagate(Variable &v, Constraint &c) override;
    coro_assignment_t::pull_type generateNextAssignmentIterator() override {
        return coro_assignment_t::pull_type(boost::bind(&GACSearchNode::generateNextAssignment, this, boost::placeholders::_1));
    }

private:
    std::map<Variable_r, std::vector<Constraint_r>> mVariableToConstraints;
    std::map<Constraint_r, std::vector<Variable_r>> mConstraintToVariables;

    std::vector<std::map<Variable_r, std::set<int>>> GAC();
    std::pair<domain_t, domain_t> splitDomain(domain_t domain);
    void generateNextAssignment(coro_assignment_t::push_type &yield) override;
    void generateAssignments(coro_int_t::push_type &yield, std::vector<Variable_r>& variables, int index, int time);
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
