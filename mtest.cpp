#include <iostream>

#include "Variable.h"

#include "Expression.h"
#include "expressions/VariableExpression.h"
#include "expressions/ConstantExpression.h"
#include "expressions/AddExpression.h"
#include "expressions/NextExpression.h"

#include "Constraint.h"
#include "constraints/EqualConstraint.h"
#include "constraints/PrimitiveNextConstraint.h"
#include "constraints/UntilConstraint.h"

#include "SearchNode.h"
#include "searchNodes/GACSearchNode.h"

#include "Solver.h"

using namespace std;

//void printStates(SearchNode &i) {
//    for (auto &state : i.generateNextStatesIterator()) {
//        cout<<"new state:\n";
//        for (auto &pair : state) {
//            cout<<"variable at "<<&(pair.first.get())<<" was assigned value "<<pair.second<<"\n";
//        }
//    }
//    cout<<"\n";
//}

void printSolution(std::set<Constraint_r> constraints) {
    Solver s(constraints, GAC_NODE, 3);
    s.solve();
    s.printTree();
    cout<<"ploop\n";
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

//    std::cout<<""<<&v_a<<"\n"<<&v_b<<"\n";
//
//    // a + b == 2a + b + 2
    //Constraint &c_1 = *new EqualConstraint(add_a_b, add_add);
//
    //InstantaneousCSP i({c_1});
    //printStates(i);

    Constraint &trivial = *new EqualConstraint(e_b, e_b);
//    GACSearchNode blah({trivial});
//    printStates(blah);
//
    Constraint &trivial2 = *new EqualConstraint(e_a, e_b);
//    GACSearchNode blah2({trivial2});
//    printStates(blah2);
//
    Constraint &trivial3 = *new EqualConstraint(add_a_constant, e_b);
//    GACSearchNode blah3({trivial3});
//    printStates(blah3);
//
//    printStates(*new GACSearchNode({*new EqualConstraint(e_b, add_a_constant)}));
//
//    printStates(*new GACSearchNode({*new EqualConstraint(add_add, *new AddExpression(add_a_b, *new ConstantExpression(3)))}));





    std::cout<<"\n\n\n";
    printSolution({trivial});
    printSolution({trivial2});
    printSolution({trivial3});
    printSolution({*new EqualConstraint(e_b, add_a_constant)});
    printSolution({*new EqualConstraint(add_add, *new AddExpression(add_a_b, *new ConstantExpression(3)))});
//    printSolution({*new PrimitiveNextConstraint(*new VariableExpression(v_a), *new VariableExpression(v_a))});
    printSolution({*new EqualConstraint(e_a, *new NextExpression(e_a))});

    Expression &e_c = *new VariableExpression(*new Variable({0, 1, 2}));
    Expression &e_d = *new VariableExpression(*new Variable({0, 1, 2, 3}));
    printSolution({*new UntilConstraint(e_c, e_d)});
}

