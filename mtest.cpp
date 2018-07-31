#include <iostream>

#include "Variable.h"

#include "Expression.h"
#include "expressions/VariableExpression.h"
#include "expressions/ConstantExpression.h"
#include "expressions/AddExpression.h"

#include "Constraint.h"
#include "constraints/EqualConstraint.h"
#include "constraints/PrimitiveNextConstraint.h"

#include "InstantSolver.h"
#include "instantSolvers/GACInstantSolver.h"

#include "Solver.h"

using namespace std;

void printStates(InstantSolver &i) {
    for (auto &state : i.generateNextStatesIterator()) {
        cout<<"new state:\n";
        for (auto &pair : state) {
            cout<<"variable at "<<&(pair.first.get())<<" was assigned value "<<pair.second<<"\n";
        }
    }
    cout<<"\n";
}

void printSolution(std::set<Constraint_r> constraints) {
    Solver s(constraints, GAC_INSTANT_SOLVER);
    s.solve();
    s.printTree();
}


int main(int argc, char **argv) {
    Variable v_a({1, 2});
    Variable v_b({1, 2, 3});

    Expression &e_a = *new VariableExpression(v_a);
    Expression &e_b = *new VariableExpression(v_b);
    Expression &constant = *new ConstantExpression(2);

//    VariableExpression e_a(v_a);
//    VariableExpression e_b(v_b);
//    ConstantExpression constant(2);
//
//    cout<<"moop";
//
    Expression &add_a_b = *new AddExpression(e_a, e_b);
    Expression &add_a_constant = *new AddExpression(e_a, constant);
    Expression &add_add = *new AddExpression(add_a_b, add_a_constant);

    std::cout<<""<<&v_a<<"\n"<<&v_b<<"\n";
//
//    // a + b == 2a + b + 2
    //Constraint &c_1 = *new EqualConstraint(add_a_b, add_add);
//
    //InstantaneousCSP i({c_1});
    //printStates(i);

    Constraint &trivial = *new EqualConstraint(e_b, e_b);
    GACInstantSolver blah({trivial});
    printStates(blah);

    Constraint &trivial2 = *new EqualConstraint(e_a, e_b);
    GACInstantSolver blah2({trivial2});
    printStates(blah2);

    Constraint &trivial3 = *new EqualConstraint(add_a_constant, e_b);
    GACInstantSolver blah3({trivial3});
    printStates(blah3);

    printStates(*new GACInstantSolver({*new EqualConstraint(e_b, add_a_constant)}));

    printStates(*new GACInstantSolver({*new EqualConstraint(add_add, *new AddExpression(add_a_b, *new ConstantExpression(3)))}));

    std::cout<<"\n\n\n";
    printSolution({trivial});
    printSolution({trivial2});
    printSolution({trivial3});
    printSolution({*new EqualConstraint(e_b, add_a_constant)});
    printSolution({*new EqualConstraint(add_add, *new AddExpression(add_a_b, *new ConstantExpression(3)))});
    printSolution({*new PrimitiveNextConstraint(*new VariableExpression(v_a), *new VariableExpression(v_a))});
}

