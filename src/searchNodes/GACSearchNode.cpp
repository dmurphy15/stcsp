#include "../../include/searchNodes/GACSearchNode.h"

#include <iterator>

#include "../../include/Constraint.h"
#include "../../include/Variable.h"

GACSearchNode::GACSearchNode(const std::set<Constraint_r>& constraints,
                             const assignment_t& historicalValues,
                             const std::vector<std::pair<std::map<Variable_r, domain_t>::const_iterator,std::map<Variable_r, domain_t>::const_iterator>>& domains,
                             int constraintSetId)
        : SearchNode(constraints, historicalValues, domains, constraintSetId)
{
    for (Constraint &c : constraints)
    {
        std::set<Variable_r> vars; c.getVariables(vars, id==0);
        mConstraintToVariables.insert({&c, std::vector<Variable_r>()});
        for (Variable &v : vars)
        {
            mConstraintToVariables[&c].push_back(v);
            // if v is not in mVariableToConstraints, this will automatically create an empty entry for it
            mVariableToConstraints[v].push_back(&c);
        }
        mConstraintPtrs.insert(&c);
    }
    for (auto &assignment : historicalValues) {
        mDomains[0][assignment.first] = {assignment.second};
    }
    mAssignments.resize(getPrefixK());
}

void GACSearchNode::generateNextAssignment(coro_assignment_t::push_type& yield)
{
    std::vector<std::map<Variable_r, std::set<int>>> removals = GAC();
    bool yieldAssignment = true;
    // only ever split domains on the first timepoint in our time period, since that's the one we're trying to assign
    std::map<Variable_r, domain_t>& firstDomains = mDomains[0];
    for (auto &p : firstDomains)
    {
        Variable &v = p.first;
        if (firstDomains[v].size() == 0)
        {
            yieldAssignment = false;
            break;
        }
        else if (firstDomains[v].size() > 1)
        {
            yieldAssignment = false;
            domain_t loDomain, hiDomain;
            splitDomain(firstDomains[v], loDomain, hiDomain);
            firstDomains[v] = loDomain;
            generateNextAssignment(yield);
            firstDomains[v] = hiDomain;
            generateNextAssignment(yield);
            // reset the domain
            firstDomains[v].insert(loDomain.begin(), loDomain.end());
            break;
        }
    }
    if (yieldAssignment)
    {
        assignment_t map;
        for (auto &p : firstDomains)
        {
            Variable &v = p.first;
            map.insert({v, *firstDomains[v].begin()});
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
        Constraint *c = *it;
        constraintQueue.erase(it);
        for (Variable &v : mConstraintToVariables[c])
        {
            bool altered = false;
            std::vector<std::set<int>> alterations = c->propagate(v, *this);
            for (int time=0; time < getPrefixK(); time++) {
                // propagate has adjusted the domain of the chosen variable
                if (alterations[time].size() > 0)
                {
                    total_alterations[time][v].insert(alterations[time].begin(), alterations[time].end());
                    altered = true;
                }
            }
            if (altered)
                constraintQueue.insert(mVariableToConstraints[v].begin(), mVariableToConstraints[v].end());
        }
    }
    return total_alterations;
}

std::vector<std::set<int>> GACSearchNode::defaultPropagate(Variable &v, Constraint &c)
{
    std::vector<std::set<int>> ret(getPrefixK());
    // get all the related variables in this arc
    std::vector<Variable_r>::iterator position = std::find(mConstraintToVariables[&c].begin(),
                                                           mConstraintToVariables[&c].end(),
                                                           v);
    std::vector<Variable_r> others(mConstraintToVariables[&c].begin(), position);
    others.insert(others.end(), ++position, mConstraintToVariables[&c].end());

    // iterate over the domain of our variable
    for (int time = 0; time < getPrefixK(); time++) {
        for (auto iter = getDomain(v, time).begin(); iter != getDomain(v, time).end(); ) {
            mAssignments[time][v] = *iter;
            if (shouldPrune(c, time, others.begin(), others.end())) {
                ret[time].insert(*iter);
                iter = pruneDomain(v, iter, time);
            } else {
                iter++;
            }
        }
    }
    return ret;
}

bool GACSearchNode::shouldPrune(Constraint& c,
                                int time,
                                std::vector<Variable_r>::iterator index,
                                std::vector<Variable_r>::iterator endIt)
{
    if (index == endIt) {
        return !c.isSatisfied(*this, time);
    }
    for (int val : mDomains[time][*index]) {
        mAssignments[time][*index] = val;
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

coro_assignment_t::pull_type GACSearchNode::generateNextAssignmentIterator() {
    return coro_assignment_t::pull_type(boost::bind(&GACSearchNode::generateNextAssignment, this, _1));
}
