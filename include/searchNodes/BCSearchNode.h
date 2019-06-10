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
                  const std::vector<std::pair<std::map<Variable_r, domain_t>::const_iterator,std::map<Variable_r, domain_t>::const_iterator>>& domains,
                  int constraintSetId=-1);
    std::vector<std::set<int>> defaultPropagate(Variable &v, Constraint &c) override;
    coro_assignment_t::pull_type generateNextAssignmentIterator() override;

private:
    // we took in a set of constraints, so duplicates are already removed. Thus we can
    // now treat each constraint as unique, and use pointers, which are faster to compare
    // than actually comparing the contents of the constraint objects
    std::map<Variable_r, std::vector<Constraint *>> mVariableToConstraints;
    std::map<Constraint *, std::vector<Variable_r>> mConstraintToVariables;

    std::set<Constraint *>mConstraintPtrs;

    std::vector<std::map<Variable_r, std::set<int>>> BC();
    void splitDomain(domain_t& inDomain, domain_t& loDomain, domain_t& hiDomain);
    void generateNextAssignment(coro_assignment_t::push_type &yield) override;
    bool shouldPrune(Constraint& c,
                     int time,
                     std::vector<Variable_r>::iterator index,
                     std::vector<Variable_r>::iterator endIt);
};
