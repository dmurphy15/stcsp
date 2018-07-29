#pragma once

#include <map>
#include <set>
#include <functional>
#include <vector>
#include "boost/config.hpp"
#include "boost/coroutine2/all.hpp"
#include "boost/bind.hpp"

class Variable;
class Constraint;

typedef boost::coroutines2::coroutine<std::map<std::reference_wrapper<Variable>, int>> coro_assignment_t;
typedef boost::coroutines2::coroutine<int> coro_int_t;

class InstantaneousCSP
{
    using Constraint_r = std::reference_wrapper<Constraint>;
    using Variable_r = std::reference_wrapper<Variable>;
    using InstantaneousCSP_r = std::reference_wrapper<InstantaneousCSP>;
public:
    // TODO add this to interface
    InstantaneousCSP(std::set<Constraint_r> constraints);
    // TODO add this to interface
    void generateNextStates(coro_assignment_t::push_type &yield); //returns a generator for iterating through the solutions
    // TODO add this to interface
    void addChildNode(std::map<Variable_r, int> assignment, InstantaneousCSP &child); //this lets the solver set individual child nodes. so the solver will get a statenode from the iterator,
                    //then look for previous equivalent statenodes. If it finds a previous one, it will add that as the
                    //child and get the next state node etc, if it does not, then it adds the new one as the child and
                    //recurs on it, before getting the next state node
    // TODO add this to interface
    std::vector<int> defaultPropagate(Variable &v, Constraint &c);

    //all constraints (and expressions) need to have a hash function so we can determine equivalence. Perhaps a function that
    //combines the recursive hashes of its direct components plus its constraint(/expression) type

    std::map<Variable_r, std::vector<int>> domains;
    std::map<Variable_r, int> assignments;
    std::map<Variable_r, std::vector<Constraint_r>> variableToConstraints;
    std::map<Constraint_r, std::vector<Variable_r>> constraintToVariables;
private:
//    Variable &getNextVariable(std::vector<std::reference_wrapper<Variable>> variables);
    std::map<Variable_r, std::vector<int>> GAC();
    std::pair<std::vector<int>, std::vector<int>> splitDomain(std::vector<int> domain);
    void generateAssignments(coro_int_t::push_type &yield, std::vector<Variable_r> variables);

    std::set<Constraint_r> mConstraints;
    std::set<Variable_r> mVariables;
//    VariableChooser mVariableChooser;
    std::map<std::map<Variable_r, int>, InstantaneousCSP_r> mChildNodes;
};

//REPLACE ALL UNORDERED_SETS with regular, ordered, sets, or at least think about it.
//One issue with sets of pointers is that it would compare identity rather than making sure the constriaints
//have the same values (are the same type and have the same member expressions). But if we are careful to
//never copy constraints after we begin solving, we may be alright
//
//future optimization - instead of iterating through all bottommost variables contained by the constraints
//directly, break them up so the expressions can create equivalent variables whose domains are sort of
//arrays whose initial elements represent combinations of values of the input variables and whose last
//element represents the expression's evaluated value - memory inefficient
//could use this for binarization to use AC-3 algorithm
