#include "../../include/searchNodes/GACSearchNode.h"

#include <iterator>
#include <vector>

#include "../../include/Constraint.h"
#include "../../include/Variable.h"

GACSearchNode::GACSearchNode(const std::set<Constraint_r>& constraints,
                           const assignment_t& historicalValues,
                           const std::vector<std::map<Variable_r, domain_t>>& domains,
                           int constraintSetId)
        : SearchNode(constraints, historicalValues, domains, constraintSetId)
{
    for (Constraint &c : constraints)
    {
        std::set<Variable_r> vars = c.getVariables();
        for (Variable &v : vars)
        {
            mVariableToConstraints[v].push_back(&c);
        }
        mConstraintPtrs.insert(&c);
    }
}

void GACSearchNode::generateNextAssignment(coro_assignment_t::push_type& yield)
{
    std::vector<std::map<Variable_r, std::set<int>>> removals = GAC();
    bool yieldAssignment = true;
    std::map<Variable_r, domain_t>& firstDomains = mDomains[0];
    for (auto &p : firstDomains)
    {
        domain_t &d = p.second;
        if (d.size() == 0)
        {
            yieldAssignment = false;
            break;
        }
        else if (d.size() > 1)
        {
            yieldAssignment = false;
            domain_t loDomain, hiDomain;
            splitDomain(d, loDomain, hiDomain);
            d = loDomain;
            generateNextAssignment(yield);
            d = hiDomain;
            generateNextAssignment(yield);
            d.insert(loDomain.begin(), loDomain.end());
            break;
        }
    }
    if (yieldAssignment)
    {
        assignment_t map;
        for (auto &p : firstDomains)
        {
            Variable &v = p.first;
            domain_t &d = p.second;
            map.insert({v, *d.begin()});
        }
        yield(map);
    }
    // reset the changes we made to the domains
    for (int i=0; i < getPrefixK(); i++) {
        for (auto const &p : removals[i]) {
            mDomains[i][p.first].insert(p.second.begin(), p.second.end());
        }
    }
}


std::vector<std::map<Variable_r, std::set<int>>> GACSearchNode::GAC()
{
    std::vector<std::map<Variable_r, std::set<int>>> total_alterations(getPrefixK());
    std::set<Constraint *> constraintQueue = mConstraintPtrs;
    while (constraintQueue.size() > 0)
    {
        auto it = constraintQueue.begin();
        Constraint *c_ptr = *it;
        constraintQueue.erase(it);
        std::map<Variable_r, std::vector<std::set<int>>> alterationsMap = c_ptr->propagate(*this);
        for (auto& pair : alterationsMap) {
            bool altered = false;
            Variable& v = pair.first;
            std::vector<std::set<int>>& alterations = pair.second;
            for (int time=0; time < getPrefixK(); time++) {
                // propagate has adjusted the domain of the chosen variable
                if (alterations[time].size() > 0)
                {
                    total_alterations[time][v].insert(alterations[time].begin(), alterations[time].end());
                    altered = true;
                }
            }
            auto& cs = mVariableToConstraints[v];
            if (altered)
                constraintQueue.insert(cs.begin(), cs.end());
        }
    }
    return total_alterations;
}

std::map<Variable_r, std::vector<std::set<int>>> GACSearchNode::defaultPropagate(Constraint &c)
{
    std::map<Variable_r, std::vector<std::set<int>>> retMap;
    int k = (id==SearchNode::ROOT_ID && c.containsFirstExpression()) ? 1 : getPrefixK();
    for (int time = 0; time < k; time++) {
        const std::set<Variable_r>& variables = c.getVariables();
        std::set<Variable_r>::iterator varIter = variables.begin();
        while (varIter != variables.end()) {
            Variable& v = *varIter;
            std::vector<std::set<int>>& ret = retMap[v];
            ret.resize(getPrefixK());
            // get all the related variables in this arc
            std::vector<Variable_r> others(variables.begin(), varIter);
            others.insert(others.end(), ++varIter, variables.end());

            // iterate over the domain of our variable
            auto& d = getDomain(v, time);
            auto& asgnmnt = mAssignments[time][v];
            for (auto iter = d.begin(); iter != d.end(); ) {
                asgnmnt = *iter;
                if (shouldPrune(c, time, others.begin(), others.end())) {
                    ret[time].insert(*iter);
                    iter = pruneDomain(v, iter, time);
                } else {
                    iter++;
                }
            }
        }
    }
    return retMap;
}

bool GACSearchNode::shouldPrune(Constraint& c,
                                int time,
                                std::vector<Variable_r>::iterator index,
                                std::vector<Variable_r>::iterator endIt)
{
    if (index == endIt) {
        return !c.isSatisfied(*this, time);
    }
    auto& asgnmnt = mAssignments[time][*index];
    for (int val : mDomains[time][*index]) {
        asgnmnt = val;
        if (!shouldPrune(c, time, std::next(index), endIt)) {
            return false;
        }
    }
    return true;
}

void GACSearchNode::splitDomain(domain_t& inDomain, domain_t& loDomain, domain_t& hiDomain)
{
    std::size_t i=0;
    int halfSize = inDomain.size() / 2;
    auto mid = inDomain.begin();
    std::advance(mid, halfSize);
    loDomain.insert(inDomain.begin(), mid);
    hiDomain.insert(mid, inDomain.end());
}
