#include "../../include/searchNodes/GACSearchNode.h"

#include <iterator>

#include "../../include/Constraint.h"
#include "../../include/Variable.h"
#include "../../include/Domain.h"

GACSearchNode::GACSearchNode(const std::set<Constraint_r>& constraints,
                             const assignment_t& historicalValues,
                             const std::vector<std::map<Variable_r, Domain>>& domains)
        : SearchNode(constraints, historicalValues, domains)
{
    for (Constraint &c : constraints)
    {
        std::set<Variable_r> vars; c.getVariables(vars);
        mConstraintToVariables.insert({c, std::vector<Variable_r>()});
        for (Variable &v : vars)
        {
            mConstraintToVariables[c].push_back(v);
            // if v is not in mVariableToConstraints, this will automatically create an empty entry for it
            mVariableToConstraints[v].push_back(c);
        }
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
    std::map<Variable_r, Domain>& firstDomains = mDomains[0];
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
            Domain loDomain, hiDomain;
            splitDomain(firstDomains[v], loDomain, hiDomain);
            firstDomains[v] = loDomain;
            generateNextAssignment(yield);
            firstDomains[v] = hiDomain;
            generateNextAssignment(yield);
            // reset the domain
            firstDomains[v].insert(loDomain);
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
            for (int &&removed : p.second) {
                mDomains[i][p.first].insert(removed);
            }
        }
    }
}


std::vector<std::map<Variable_r, std::set<int>>> GACSearchNode::GAC()
{
    std::vector<std::map<Variable_r, std::set<int>>> total_alterations(getPrefixK());
    std::set<Constraint_r> constraintQueue = mConstraints;
    while (constraintQueue.size() > 0)
    {
        auto it = constraintQueue.begin();
        Constraint &c = *it;
        constraintQueue.erase(it);
        for (Variable &v : mConstraintToVariables[c])
        {
            bool altered = false;
            std::vector<std::set<int>> alterations = c.propagate(v, *this);
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
    std::vector<Variable_r> others = mConstraintToVariables[c];
    std::vector<Variable_r>::iterator position = std::find(others.begin(), others.end(), v);
    if (position != others.end()) others.erase(position);

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

void GACSearchNode::splitDomain(Domain& inDomain, Domain& loDomain, Domain& hiDomain)
{
    int size = inDomain.size();
    loDomain.insert(inDomain.slice(0, size / 2));
    hiDomain.insert(inDomain.slice(size / 2, size));
}

coro_assignment_t::pull_type GACSearchNode::generateNextAssignmentIterator() {
    return coro_assignment_t::pull_type(boost::bind(&GACSearchNode::generateNextAssignment, this, _1));
}
