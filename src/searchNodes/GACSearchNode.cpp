#include "../../include/searchNodes/GACSearchNode.h"

#include <iterator>

#include "../../include/Constraint.h"
#include "../../include/Variable.h"

GACSearchNode::GACSearchNode(std::set<Constraint_r> constraints, std::map<Variable_r, int> historicalValues, std::vector<std::map<Variable_r, domain_t>> domains)
        : SearchNode(constraints, historicalValues, domains)
{
    for (Constraint &c : constraints)
    {
        std::set<Variable_r> vars = c.getVariables();
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
            domain_t lo_domain;
            domain_t hi_domain;
            std::tie(lo_domain, hi_domain) = splitDomain(firstDomains[v]);
            firstDomains[v] = lo_domain;
            generateNextAssignment(yield);
            firstDomains[v] = hi_domain;
            generateNextAssignment(yield);
            // reset the domain
            firstDomains[v].insert(lo_domain.begin(), lo_domain.end());
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
        std::map<Variable_r, domain_t>& domains = mDomains[time];
        for (auto iter = domains[v].begin(); iter != domains[v].end(); ) {
            mAssignments[time][v] = *iter;
            bool prune = true;
            // make sure that for each element of our domain, there exists an assignment of the
            // other variables that is consistent with this constraint
            coro_int_t::pull_type var_assignments(boost::bind(&GACSearchNode::generateAssignments, this, boost::placeholders::_1, others, 0, time));
            for (int &_ : var_assignments)
            {
                if (c.isSatisfied(*this, time))
                {
                    prune = false;
                    break;
                }
            }
            // if no consistent assignment was found, remove this value from our domain
            if (prune)
            {
                ret[time].insert(*iter);
                iter = domains[v].erase(iter);
            } else
            {
                iter++;
            }
        }
    }
    // just to avoid screwing things up, let's clear all the assignments we might have made in this process
    for (int i=0; i < getPrefixK(); i++) {
        mAssignments[i].clear();
    }
    return ret;
}

// iterates through all assignments of the given variables
void GACSearchNode::generateAssignments(coro_int_t::push_type& yield,
                                           std::vector<Variable_r>& variables,
                                           int index,
                                           int time)
{
    if ((int) variables.size() == index)
    {
        yield(0);
        return;
    }
    for (int val : mDomains[time][variables[index]])
    {
        mAssignments[time][variables[index]] = val;
        generateAssignments(yield, variables, index+1, time);
    }
}

std::pair<domain_t, domain_t> GACSearchNode::splitDomain(domain_t& domain)
{
    domain_t split_lo;
    domain_t split_hi;
    std::size_t i=0;
    for (auto it = domain.begin(); it != domain.end(); it++, i++) {
        if (i < domain.size() / 2) {
            split_lo.insert(*it);
        } else {
            split_hi.insert(*it);
        }
    }
    return std::make_pair(split_lo, split_hi);
}

coro_assignment_t::pull_type GACSearchNode::generateNextAssignmentIterator() {
    return coro_assignment_t::pull_type(boost::bind(&GACSearchNode::generateNextAssignment, this, _1));
}
