#pragma once
#include <vector>
#include <unordered_set>

#include "Variable.h"

class Solver;

/*
 * Interface that all constraints should satisfy.
 *
 * normalize takes a list for constraints an new variables that would be equivalent to
 *  the current constraint. With the resulting lists from a call to normalize, a solver
 *  should be able to delete the constraint and add the new constraints and variables
 *
 * isSatisfied takes a time and returns 1 if the constraint is satisfied then, and 0 otherwise.
 *
 */

class Constraint
{
public:
    /* for now I'm having all implemented constraints call this in their constructors; in the future, should
     * just have the solver do it when you do solver.addConstraint or something*/
    void init() {
        for (Variable *v: getVariables())
        {
            v->addConstraint(this);
        }
    }
    /* same here */
    void deinit() {
        for (Variable *v: getVariables()) {
            v->removeConstraint(this);
        }
    }
    /* the consts here mean that the pointer cannot be made to point somewhere else;
     * it essentially forces you to allocate space for the set ahead of time */
    virtual void normalize(std::unordered_set<Constraint *> *const constraintList,
                           std::unordered_set<Variable *> *const variableList) const = 0;
    virtual int isSatisfied(int time) const = 0;
    virtual std::unordered_set<Variable *> getVariables() const = 0;

    virtual std::unordered_set<Constraint *> propagate(int time, Variable *v) const = 0;

    void addSolver(Solver *solver);
private:
    Solver *mOwnerSolver;
};