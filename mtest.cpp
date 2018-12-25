#include <iostream>
#include <chrono>

#include "include/all.h"

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
    auto start = chrono::system_clock::now();
    Solver s(GAC_NODE, 3, constraints);
    s.solve();
    auto ending = chrono::system_clock::now();
    std::chrono::duration<double> elapsed = ending - start;
    s.printTree();
    cout<<"ploop "<< elapsed.count() <<"\n";
}


int main(int argc, char **argv) {
    Variable v_a(Domain(std::set<int>{1, 2}));
    Variable v_b(Domain({1, 2, 3}));

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




    printSolution({*new EqualConstraint(*new FirstExpression(e_a), e_b)});

    std::cout<<"\n\n\n";
    printSolution({trivial});
    printSolution({trivial2});
    printSolution({trivial3});
    printSolution({*new EqualConstraint(e_b, add_a_constant)});
    printSolution({*new EqualConstraint(add_add, *new AddExpression(add_a_b, *new ConstantExpression(3)))});
//    printSolution({*new PrimitiveNextConstraint(*new VariableExpression(v_a), *new VariableExpression(v_a))});
    printSolution({*new EqualConstraint(e_a, *new NextExpression(e_a))});

    Expression &e_c = *new VariableExpression(*new Variable(Domain({0, 1, 2})));
    Expression &e_d = *new VariableExpression(*new Variable(Domain({0, 1, 2, 3})));
    printSolution({*new UntilConstraint(e_c, e_d)});

    printSolution({*new EqualConstraint(*new FirstExpression(e_a), e_b)});

    VariableExpression *time = new VariableExpression(*new Variable(Domain({-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5})));
    VariableExpression *position = new VariableExpression(*new Variable(Domain({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25})));
    VariableExpression *velocity = new VariableExpression(*new Variable(Domain({-10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10})));
    VariableExpression *acceleration = new VariableExpression(*new Variable(Domain({-25, -24, -23, -22, -21, -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 1, 2})));

    // lol it says that this is unsatisfiable, and ITS CORRECT since I say that time must always increase, but it has a
            // finite domain so eventually it will always be unable to increase further
    printSolution({*new EqualConstraint(*position, *new MultiplyExpression(*time, *time)),
                  *new EqualConstraint(*velocity,
                                       *new AddExpression(*new NextExpression(*position),
                                                          *new MultiplyExpression(*position, *new ConstantExpression(-1)))),
                  *new EqualConstraint(*acceleration,
                                      *new AddExpression(*new NextExpression(*velocity),
                                                         *new MultiplyExpression(*velocity, *new ConstantExpression(-1)))),
                  *new EqualConstraint(*new NextExpression(*time), *new AddExpression(*time, *new ConstantExpression(1))),
                  *new EqualConstraint(*new FirstExpression(*time), *new ConstantExpression(-5))});

    // fixing that naively with an until constraint
    VariableExpression *switchExp = new VariableExpression(*new Variable(Domain(std::set<int>{0, 1})));
    printSolution({*new EqualConstraint(*position, *new MultiplyExpression(*time, *time)),
                   *new EqualConstraint(*velocity,
                                        *new AddExpression(*new NextExpression(*position),
                                                           *new MultiplyExpression(*position, *new ConstantExpression(-1)))),
                   *new EqualConstraint(*acceleration,
                                        *new AddExpression(*new NextExpression(*velocity),
                                                           *new MultiplyExpression(*velocity, *new ConstantExpression(-1)))),
                   *new EqualConstraint(*new NextExpression(*time),
                                        *new AddExpression(*time, *switchExp)),
                   *new EqualConstraint(*new FirstExpression(*time), *new ConstantExpression(-5)),
                   *new UntilConstraint(*switchExp, *new EqualExpression(*time, *new ConstantExpression(5)))});
}

