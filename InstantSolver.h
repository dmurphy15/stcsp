#pragma once

#include <map>
#include <set>
#include <vector>
#include <functional>
#include "boost/coroutine2/all.hpp"
#include "boost/bind.hpp"

class Variable;
using Variable_r = std::reference_wrapper<Variable>;
class Constraint;
using Constraint_r = std::reference_wrapper<Constraint>;
class InstantSolver;
using InstantSolver_r = std::reference_wrapper<InstantSolver>;

typedef boost::coroutines2::coroutine<std::map<std::reference_wrapper<Variable>, int>> coro_assignment_t;
typedef boost::coroutines2::coroutine<int> coro_int_t;


class InstantSolver
{
public:
    InstantSolver(std::set<Constraint_r> constraints, std::map<Variable_r, int> inputAssignments);

    int getAssignment(Variable &v);
    std::set<int> getDomain(Variable &v);
    void setDomain(Variable &v, std::set<int> domain);

    std::set<Constraint_r> getConstraints();

    virtual coro_assignment_t::pull_type generateNextStatesIterator() = 0;

    //this lets the solver set individual child nodes. so the solver will get a statenode from the iterator,
    //then look for previous equivalent statenodes. If it finds a previous one, it will add that as the
    //child and get the next state node etc, if it does not, then it adds the new one as the child and
    //recurs on it, before getting the next state node
    void addChildNode(InstantSolver &child, std::map<Variable_r, int> assignment);
    void removeLastChildNode();
    std::vector<std::pair<InstantSolver_r, std::map<Variable_r, int>>> getChildNodes();

    virtual std::set<int> defaultPropagate(Variable &v, Constraint &c) = 0;

    friend bool operator==(InstantSolver &lhs, InstantSolver &rhs);
protected:
    // cast this to a coroutine, and then you can iterate through the next states
    virtual void generateNextStates(coro_assignment_t::push_type &yield) = 0;

    std::map<Variable_r, int> mAssignments;
    std::map<Variable_r, std::set<int>> mDomains;
    std::set<Constraint_r> mConstraints;
    std::map<Variable_r, int> mInputAssignments;
    std::vector<std::pair<InstantSolver_r, std::map<Variable_r, int>>> mChildNodes;
};