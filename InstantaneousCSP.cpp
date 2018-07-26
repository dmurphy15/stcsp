#include "InstantaneousCSP.h"

#include "Constraint.h"
#include "Variable.h"

#include <iostream>

InstantaneousCSP::InstantaneousCSP(std::set<Constraint *, ConstraintLtComparator> constraints)
{
    mConstraints = constraints;
    for (Constraint *c : constraints)
    {
        std::set<Variable *> vars = c->getVariables();
        mVariables.insert(vars.begin(), vars.end());
        constraintToVariables.insert({c, std::vector<Variable *>()});
        for (Variable *v : vars)
        {
            constraintToVariables[c].push_back(v);
            if (variableToConstraints.count(v) == 0) variableToConstraints.insert({v, {}});
            variableToConstraints[v].push_back(c);
            domains.insert({v, v->getInitialDomain()});
        }
    }
}

void InstantaneousCSP::generateNextStates(coro_assignment_t::push_type& yield)
{
    GAC();
    bool yieldAssignment = true;
    for (Variable *v : mVariables)
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
            generateNextStates(yield);
            domains[v] = hi_domain;
            generateNextStates(yield);
        }
    }
    if (yieldAssignment)
    {
        std::map<Variable *, int> map;
        for (Variable *v : mVariables)
        {
            map.insert({v, domains[v][0]});
        }
        yield(map);
    }
}


void InstantaneousCSP::GAC()
{
    std::set<Constraint *, ConstraintLtComparator> constraintQueue = mConstraints;
    while (constraintQueue.size() > 0)
    {
        auto it = constraintQueue.begin();
        Constraint *c = *it;
        constraintQueue.erase(it);
        for (Variable *v : constraintToVariables[c])
        {
            // propagate has adjusted the domain of the chosen variable
            if (c->propagate(v, this))
            {
                constraintQueue.insert(variableToConstraints[v].begin(), variableToConstraints[v].end());
            }
        }
    }
}

bool InstantaneousCSP::defaultPropagate(Variable *v, Constraint *c)
{
    int ret = false;
    // get all the related variables in this arc
    std::vector<Variable *> others = constraintToVariables[c];
    std::vector<Variable *>::iterator position = std::find(others.begin(), others.end(), v);
    if (position != others.end()) others.erase(position);

    // iterate over the domain of our variable
    for (auto iter = domains[v].begin(); iter != domains[v].end(); ) {
        assignments[v] = *iter;
        bool prune = true;
        // make sure that for each element of our domain, there exists an assignment of the
        // other variables that is consistent with this constraint
        coro_int_t::pull_type assignments(boost::bind(&InstantaneousCSP::generateAssignments, this, boost::placeholders::_1, others));
        for (int &_ : assignments)
        {
            if (c->isSatisfied(this))
            {
                prune = false;
                break;
            }
        }
        // if no consistent assignment was found, remove this value from our domain
        if (prune)
        {
            iter = domains[v].erase(iter);
            // indicate that we have changed some of the domains
            ret = true;
        } else
        {
            iter++;
        }
    }
    // just to avoid screwing things up, let's clear all the assignments we might have made in this process
    assignments.clear();
    return ret;
}

// iterates through all assignments of the given variables
void InstantaneousCSP::generateAssignments(coro_int_t::push_type& yield, std::vector<Variable *> variables)
{
    if (variables.size() == 0)
    {
        yield(0);
        return;
    }
    std::vector<Variable *> rest(variables.begin() + 1, variables.end());
    for (int val : domains[variables[0]])
    {
        assignments[variables[0]] = val;
        generateAssignments(yield, rest);
    }
}

std::pair<std::vector<int>, std::vector<int>> InstantaneousCSP::splitDomain(std::vector<int> domain)
{
    std::size_t const half_size = domain.size() / 2;
    std::vector<int> split_lo(domain.begin(), domain.begin() + half_size);
    std::vector<int> split_hi(domain.begin() + half_size, domain.end());
    return std::make_pair(split_lo, split_hi);
}

void InstantaneousCSP::addChildNode(std::map<Variable *, int> assignment, InstantaneousCSP child)
{
    mChildNodes.insert({assignment, child});
}