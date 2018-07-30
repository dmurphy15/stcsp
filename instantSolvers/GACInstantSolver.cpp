#include "GACInstantSolver.h"

#include "../Constraint.h"
#include "../Variable.h"

#include <iostream>

GACInstantSolver::GACInstantSolver(std::set<Constraint_r> constraints, std::map<Variable_r, int> inputAssignments)
        : InstantSolver(constraints, inputAssignments)
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
            mDomains.insert({v, v.getInitialDomain()});
        }
    }
    for (auto &assignment : inputAssignments) {
        mDomains[assignment.first] = {assignment.second};
    }
}

void GACInstantSolver::generateNextStates(coro_assignment_t::push_type& yield)
{
    std::map<Variable_r, std::vector<int>> removals = GAC();
    bool yieldAssignment = true;
    for (auto &p : mDomains)
    {
        Variable &v = p.first;
        if (mDomains[v].size() == 0)
        {
            yieldAssignment = false;
            break;
        }
        else if (mDomains[v].size() > 1)
        {
            yieldAssignment = false;
            std::vector<int> lo_domain;
            std::vector<int> hi_domain;
            std::tie(lo_domain, hi_domain) = splitDomain(mDomains[v]);
            mDomains[v] = lo_domain;
            generateNextStates(yield);
            mDomains[v] = hi_domain;
            generateNextStates(yield);
            break;
        }
    }
    if (yieldAssignment)
    {
        std::map<Variable_r, int> map;
        for (auto &p : mDomains)
        {
            Variable &v = p.first;
            map.insert({v, mDomains[v][0]});
        }
        yield(map);
    }
    // reset the changes we made to the domains
    for (auto const &p : removals) {
        mDomains[p.first].insert(mDomains[p.first].end(), p.second.begin(), p.second.end());
    }
}


std::map<Variable_r, std::vector<int>> GACInstantSolver::GAC()
{
    std::map<Variable_r, std::vector<int>> total_alterations;
    std::set<Constraint_r> constraintQueue = mConstraints;
    while (constraintQueue.size() > 0)
    {
        auto it = constraintQueue.begin();
        Constraint &c = *it;
        constraintQueue.erase(it);
        for (Variable &v : mConstraintToVariables[c])
        {
            std::vector<int> alterations = c.propagate(v, *this);
            // propagate has adjusted the domain of the chosen variable
            if (alterations.size() > 0)
            {
                total_alterations[v].insert(total_alterations[v].end(), alterations.begin(), alterations.end());
                constraintQueue.insert(mVariableToConstraints[v].begin(), mVariableToConstraints[v].end());
            }
        }
    }
    return total_alterations;
}

std::vector<int> GACInstantSolver::defaultPropagate(Variable &v, Constraint &c)
{
    std::vector<int> ret;
    // get all the related variables in this arc
    std::vector<Variable_r> others = mConstraintToVariables[c];
    std::vector<Variable_r>::iterator position = std::find(others.begin(), others.end(), v);
    if (position != others.end()) others.erase(position);

    // iterate over the domain of our variable
    for (auto iter = mDomains[v].begin(); iter != mDomains[v].end(); ) {
        mAssignments[v] = *iter;
        bool prune = true;
        // make sure that for each element of our domain, there exists an assignment of the
        // other variables that is consistent with this constraint
        coro_int_t::pull_type var_assignments(boost::bind(&GACInstantSolver::generateAssignments, this, boost::placeholders::_1, others));
        for (int &_ : var_assignments)
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
            ret.push_back(*iter);
            iter = mDomains[v].erase(iter);
        } else
        {
            iter++;
        }
    }
    // just to avoid screwing things up, let's clear all the assignments we might have made in this process
    mAssignments.clear();
    return ret;
}

// iterates through all assignments of the given variables
void GACInstantSolver::generateAssignments(coro_int_t::push_type& yield, std::vector<Variable_r> variables)
{
    if (variables.size() == 0)
    {
        yield(0);
        return;
    }
    std::vector<Variable_r> rest(variables.begin() + 1, variables.end());
    for (int val : mDomains[variables[0]])
    {
        mAssignments[variables[0]] = val;
        generateAssignments(yield, rest);
    }
}

std::pair<std::vector<int>, std::vector<int>> GACInstantSolver::splitDomain(std::vector<int> domain)
{
    std::size_t const half_size = domain.size() / 2;
    std::vector<int> split_lo(domain.begin(), domain.begin() + half_size);
    std::vector<int> split_hi(domain.begin() + half_size, domain.end());
    return std::make_pair(split_lo, split_hi);
}
//
//coro_assignment_t::pull_type GACInstantSolver::generateNextStatesIterator()
//{
//return coro_assignment_t::pull_type(boost::bind(&GACInstantSolver::generateNextStates, this, boost::placeholders::_1));
//}