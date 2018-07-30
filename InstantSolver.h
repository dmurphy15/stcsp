#pragma once

#include <map>
#include <set>
#include <vector>
#include <functional>
#include "boost/coroutine2/all.hpp"
#include "boost/bind.hpp"

class Variable;
class Constraint;
class InstantSolver;
using InstantSolver_r = std::reference_wrapper<InstantSolver>;

typedef boost::coroutines2::coroutine<std::map<std::reference_wrapper<Variable>, int>> coro_assignment_t;
typedef boost::coroutines2::coroutine<int> coro_int_t;


class InstantSolver
{
    using Variable_r = std::reference_wrapper<Variable>;
    using Constraint_r = std::reference_wrapper<Constraint>;
public:
    InstantSolver() = delete;
    InstantSolver(std::set<Constraint_r> constraints, std::map<Variable_r, int> inputAssignments) {
        mConstraints = constraints;
        mInputAssignments = inputAssignments;
    }

    int getAssignment(Variable &v) {
        return mAssignments.at(v);
    }
    std::vector<int> getDomain(Variable &v) {
        return mDomains.at(v);
    }
    void setDomain(Variable &v, std::vector<int> domain) {
        mDomains.at(v) = domain;
    }

    std::set<Constraint_r> getConstraints() {
        return mConstraints;
    }

    virtual coro_assignment_t::pull_type generateNextStatesIterator() = 0;

    //this lets the solver set individual child nodes. so the solver will get a statenode from the iterator,
    //then look for previous equivalent statenodes. If it finds a previous one, it will add that as the
    //child and get the next state node etc, if it does not, then it adds the new one as the child and
    //recurs on it, before getting the next state node
    void addChildNode(std::map<Variable_r, int> assignment, InstantSolver &child) {
        mChildNodes.insert({assignment, child});
    }
    std::map<std::map<Variable_r, int>, InstantSolver_r> getChildNodes() {
        return mChildNodes;
    };
protected:
    virtual std::vector<int> defaultPropagate(Variable &v, Constraint &c) = 0;
    // cast this to a coroutine, and then you can iterate through the next states
    virtual void generateNextStates(coro_assignment_t::push_type &yield) = 0;

    std::map<Variable_r, int> mAssignments;
    std::map<Variable_r, std::vector<int>> mDomains;
    std::set<Constraint_r> mConstraints;
    std::map<Variable_r, int> mInputAssignments;
    std::vector<InstantSolver_r> mChildNodes;
};