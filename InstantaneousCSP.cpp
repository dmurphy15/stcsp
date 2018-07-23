#include "InstantaneousCSP.h"

#include <algorithm>

#include "Constraint.h"
#include "Variable.h"

InstantaneousCSP::InstantaneousCSP(std::set <Constraint> constraints)
{
    mConstraints = constraints;
    for (Constraint &c : constraints)
    {
        std::set<std::reference_wrapper<Variable>> vars = c.getVariables();
        mVariables.insert(vars.begin(), vars.end());
        constraintToVariables.insert({c, {}});
        for (Variable &v : vars)
        {
            constraintToVariables[c].push_back(v);
            if (variableToConstraints.count(v) == 0) variableToConstraints.insert({v, {}});
            variableToConstraints[v].push_back(c);
            domains.insert({v, v.getInitialDomain()});
        }
    }
}

//Variable& InstantaneousCSP::getNextVariable(std::vector<std::reference_wrapper<Variable>> variables)
//{
//
//}

generator<std::map<std::reference_wrapper<Variable>, int>> InstantaneousCSP::getNextState()
{
    GAC();
    bool yieldAssignment = true;
    for (Variable &v : mVariables)
    {
        if (domains[v].size() == 0)
        {
            yieldAssignment = false;
            break;
        }
        else if (domains[v].size() > 1)
        {
            yieldAssignment = false;
            std::vector<int> lo_domain;
            std::vector<int> hi_domain;
            std::tie(lo_domain, hi_domain) = splitDomain(domains[v]);
            domains[v] = lo_domain;
            for (std::map<std::reference_wrapper<Variable>, int> &assignment : getNextState())
            {
                co_yield(assignment);
            }
            domains[v] = hi_domain;
            for (std::map<std::reference_wrapper<Variable>, int> &assignment : getNextState())
            {
                co_yield(assignment);
            }
        }
    }
    if (yieldAssignment)
    {
        std::map<std::reference_wrapper<Variable>, int> map;
        for (Variable &v : mVariables)
        {
            map.insert({v, domains[v][0]});
        }
        co_yield map;
    }
}

void InstantaneousCSP::GAC()
{
    std::set<Constraint> constraintQueue = mConstraints;
    while (constraintQueue.size() > 0)
    {
        auto it = constraintQueue.begin();
        constraintQueue.erase(it);
        Constraint c = *it;
        for (Variable &v : constraintToVariables[c])
        {
            // propagate has adjusted the domain of the chosen variable
            if (c.propagate(v, *this))
            {
                constraintQueue.insert(variableToConstraints[v].begin(), variableToConstraints.end());
            }
        }
    }
}

void InstantaneousCSP::defaultPropagate(Variable &v, Constraint c)
{
    // get all the related variables in this arc
    std::vector<std::reference_wrapper<Variable>> others = constraintToVariables[v];
    std::vector<int>::iterator position = std::find(others.begin(), others.end(), v);
    if (position != others.end()) others.erase(position);

    // iterate over the domain of our variable
    for (auto iter = domains[v].begin(); iter != domains[v].end(); ) {
        assignments[v] = *iter;
        bool prune = true;
        // make sure that for each element of our domain, there exists an assignment of the
        // other variables that is consistent with this constraint
        for (int _ : generateAssignments(others))
        {
            if (c.isSatisfied(*this))
            {
                prune = false;
                break;
            }
        }
        // if no consistent assignment was found, remove this value from our domain
        if (prune)
        {
            iter = m_vPaths.erase(iter);
        } else
        {
            iter++;
        }
    }
    // just to avoid screwing things up, let's clear all the assignments we might have made in this process
    assignments.clear();
}

// iterates through all assignments of the given variables
generator<int> InstantaneousCSP::generateAssignments(std::vector<std::reference_wrapper<Variable>> variables)
{
    if (variables.size() == 0)
    {
        co_yield 0;
    }
    std::vector<std::reference_wrapper<Variable>> rest(variables.begin() + 1, variables.end());
    for (int val : domains[variables[0]])
    {
        assignments[variables[0]] = val;
        for (int _ : generateAssignments(rest))
        {
            co_yield 0;
        }
    }
}

std::pair<std::vector<int>, std::vector<int>> InstantaneousCSP::splitDomain(std::vector<int> domain)
{
    std::size_t const half_size = domain.size() / 2;
    std::vector<int> split_lo(lines.begin(), lines.begin() + half_size);
    std::vector<int> split_hi(lines.begin() + half_size, lines.end());
    return std::make_pair(split_lo, split_hi);
}

void InstantaneousCSP::addChildNode(std::map<int> assignment, InstantaneousCSP &child)
{
    mConstraints.insert({assignment, child});
}