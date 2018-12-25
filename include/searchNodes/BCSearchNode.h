#pragma once

#include "../SearchNode.h"

/**
 * this depends on domain_t being sorted; when we iterate over it, we go in ascending order
 */
class BCSearchNode : public SearchNode
{
public:
    BCSearchNode(const std::set<Constraint_r>& constraints,
                  const assignment_t& historicalValues,
                  const std::vector<std::map<Variable_r, Domain>>& domains);
    std::vector<std::set<int>> defaultPropagate(Variable &v, Constraint &c) override;
    coro_assignment_t::pull_type generateNextAssignmentIterator() override;

private:
    std::map<Variable_r, std::vector<Constraint_r>> mVariableToConstraints;
    std::map<Constraint_r, std::vector<Variable_r>> mConstraintToVariables;

    std::vector<std::map<Variable_r, std::set<int>>> BC();
    void splitDomain(Domain& inDomain, Domain& loDomain, Domain& hiDomain);
    void generateNextAssignment(coro_assignment_t::push_type &yield) override;
    bool shouldPrune(Constraint& c,
                     int time,
                     std::vector<Variable_r>::iterator index,
                     std::vector<Variable_r>::iterator endIt);
};
