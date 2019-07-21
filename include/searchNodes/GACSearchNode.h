#pragma once

#include "../SearchNode.h"
/**
 * A SearchNode implementation that uses Generalized Arc Consistency for constraint propagation.
 */
class GACSearchNode : public SearchNode
{
public:
    GACSearchNode(const std::set<Constraint_r>& constraints,
                  const assignment_t& historicalValues,
                  const std::vector<std::map<Variable_r, domain_t>>& domains,
                  int constraintSetId=-1);
    std::map<Variable_r, std::vector<std::set<int>>> defaultPropagate(Constraint &c) override;

private:
    std::map<Variable_r, std::vector<Constraint *>> mVariableToConstraints;

    std::set<Constraint *>mConstraintPtrs;

    std::vector<std::map<Variable_r, std::set<int>>> GAC();
    void splitDomain(domain_t& inDomain, domain_t& loDomain, domain_t& hiDomain);
    void generateNextAssignment(coro_assignment_t::push_type &yield) override;
    bool shouldPrune(Constraint& c,
                     int time,
                     std::vector<Variable_r>::iterator index,
                     std::vector<Variable_r>::iterator endIt);
};