#pragma once

#include "../SearchNode.h"

/**
 * A SearchNode implementation that uses Bounds Consistency for constraint propagation. This assumes that domains
 * are sorted from low to high.
 */
class BCSearchNode : public SearchNode
{
public:
    BCSearchNode(const std::set<Constraint_r>& constraints,
                  const assignment_t& historicalValues,
                  const std::vector<std::map<Variable_r, domain_t>>& domains,
                  int constraintSetId=-1);
    std::map<Variable_r, std::vector<std::set<int>>> defaultPropagate(Constraint &c) override;

private:
    // we took in a set of constraints, so duplicates are already removed. Thus we can
    // now treat each constraint as unique, and use pointers, which are faster to compare
    // than actually comparing the contents of the constraint objects
    std::map<Variable_r, std::vector<Constraint *>> mVariableToConstraints;

    std::set<Constraint *>mConstraintPtrs;

    std::vector<std::map<Variable_r, std::set<int>>> BC();
    void splitDomain(domain_t& inDomain, domain_t& loDomain, domain_t& hiDomain);
    void generateNextAssignment(coro_assignment_t::push_type &yield) override;
    bool shouldPrune(Constraint& c,
                     int time,
                     std::vector<Variable_r>::iterator index,
                     std::vector<Variable_r>::iterator endIt);
};
