#include <iostream>

#include "Variable.h"

#include "Expression.h"
#include "expressions/VariableExpression.h"
#include "expressions/ConstantExpression.h"
#include "expressions/AddExpression.h"

#include "Constraint.h"
#include "constraints/EqualConstraint.h"

#include "InstantaneousCSP.h"

using namespace std;

void printStates(InstantaneousCSP i) {
    coro_assignment_t::pull_type nextStates(boost::bind(&InstantaneousCSP::generateNextStates, &i, boost::placeholders::_1));
    for (auto &state : nextStates) {
        cout<<"new state:\n";
        for (auto &pair : state) {
            cout<<"variable at "<<pair.first<<" was assigned value "<<pair.second<<"\n";
        }
    }
    cout<<"\n";
}


int main(int argc, char **argv) {
    Variable *v_a = new Variable({1, 2});
    Variable *v_b = new Variable({1, 2, 3});

    Expression *e_a = new VariableExpression(v_a);
    Expression *e_b = new VariableExpression(v_b);
    Expression *constant = new ConstantExpression(2);

//    VariableExpression e_a(v_a);
//    VariableExpression e_b(v_b);
//    ConstantExpression constant(2);
//
//    cout<<"moop";
//
    Expression *add_a_b = new AddExpression(e_a, e_b);
    Expression *add_a_constant = new AddExpression(e_a, constant);
    Expression *add_add = new AddExpression(add_a_b, add_a_constant);

    // a + b == 2a + b + 2
    Constraint *c_1 = new EqualConstraint(add_a_b, add_add);

    InstantaneousCSP i({c_1});
    printStates(i);

    Constraint *trivial = new EqualConstraint(e_b, e_b);
    InstantaneousCSP blah({trivial});
    printStates(blah);

    Constraint *trivial2 = new EqualConstraint(e_a, e_b);
    InstantaneousCSP blah2({trivial2});
    printStates(blah2);
}