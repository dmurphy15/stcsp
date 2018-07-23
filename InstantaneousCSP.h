#pragma once

#include <map>
#include <set>
#include <functional>
#include <vector>
// implements a generator template
#include "generator.h"

class Constriant;
class Variable;

class InstantaneousCSP
{
public:
    // TODO add this to interface
    InstantaneousCSP(std::set<Constraint> constraints);
    // TODO add this to interface
    generator<std::map<std::reference_wrapper<Variable>, int>> getNextState(); //this will be the iterator
    // TODO add this to interface
    void addChildNode(std::map<std::reference_wrapper<Variable>, int> assignment, InstantaneousCSP &child); //this lets the solver set individual child nodes. so the solver will get a statenode from the iterator,
                    //then look for previous equivalent statenodes. If it finds a previous one, it will add that as the
                    //child and get the next state node etc, if it does not, then it adds the new one as the child and
                    //recurs on it, before getting the next state node
    // TODO add this to interface
    void defaultPropagate(Variable &v, Constraint c);

    //all constraints (and expressions) need to have a hash function so we can determine equivalence. Perhaps a function that
    //combines the recursive hashes of its direct components plus its constraint(/expression) type

    std::map<std::reference_wrapper<Variable>, std::vector<int>> domains;
    std::map<std::reference_wrapper<Variable>, int> assignments;
    std::map<std::reference_wrapper<Variable>, std::vector<Constraint>> variableToConstraints;
    std::map<Constraint, std::vector<std::reference_wrapper<Variable>>> constraintToVariables;
private:
//    Variable &getNextVariable(std::vector<std::reference_wrapper<Variable>> variables);

    void GAC();
    std::pair<std::vector<int>, std::vector<int>> splitDomain(std::vector<int> domain);
    generator<int> generateAssignments(std::vector<std::reference_wrapper<Variable>> variables);

    std::set<Constraint> mConstraints;
    std::set<std::reference_wrapper<Variable>> mVariables;
//    VariableChooser mVariableChooser;
    std::vector<std::pair<std::map<std::reference_wrapper<Variable>, int>,
                            std::reference_wrapper<InstantaneousCSP>>> mChildNodes;
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
