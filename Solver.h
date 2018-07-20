#pragma once

#include <vector>

class Constraint;
class Variable;
class InstantaneousCSP;

class Solver
{
public:
    Solver();
    // copy the constraint over, set its owner solver to us
    void addConstraint(Constraint *c) const;
    // copy the variable over
    void addVariable(Variable *v) const;
    void solve() const;

    std::unordered_set<Constraint *> defaultPropagate(int time, Variable *v) const;
private:
    // make an
    branch



    std::vector<Constraint *> mConstraints;
    std::vector<Variable *> mVariables;
    StateNode *stateTree;
};