# stcsp
Stream Constraint Satisfaction Problem solver. 

## Basic Usage

To make the program-parsing executable, simply "make" in the directory, resulting in ./bin/stcsp as the executable. Compilation requires working versions of lex and yacc, as well as the boost coroutine and context libraries.

Syntax:

./bin/stcsp input_filename

This should work for any of the test programs provided (take a look at them to see how to specify your own constraint programs). The default uses a GACRangeDomain as the underlying domain data structure, a BCSearchNode as the search node type, and a prefixK of 2.

Solution automata are printed to the terminal as well as written to the file solusions.dot in the DOT graph description language. There are plenty of software freely available to plot the automata from a .dot file. For example, Graphviz is available for Macs, and I find the linux command "sfdp -x -Goverlap=scale -Tpng solutions.dot > solutions.png" to be very helpful.

## Usage with Custom Scripts

You can also make your own constraint program directly using the classes provided. Typing "make convenience" will build a convenience header at include/all.h, which includes the necessary classes. Take a look at mtest.cpp to see how you can write your own script without having to use the program parser. Then replace "mtest" in the makefile with the name of your executable, and type "make build" to build your own executable.

In your custom script, you will need to do something like:

1. include all.h
2. construct the variables for your program, feeding them their initial domains
3. wrap the variables within VariableExpressions, and optionally relate them in more complex ways using Expressions
4. construct Constraints that will constrain those expressions (note that if a variable is completely unconstrained, the solver will not notice it, since we implicitly know that it could take any value at any state in the solution graph)
5. construct a solver, specifying the type of SearchNode to use and the value of prefixK to use, and give it your constraints
6. call solver.solve()
7. get the solution in the form of a graph of SearchNodes by calling solver.getSolutionGraph(), or write it to solutions.dot by calling solver.writeGraph()

## Extending the Framework

This framework was designed to be extensible, so that developers can easily define new classes and algorithms to efficiently solve more diverse constraint programs.

To get started, it may help to at least read the documentation in Variable.h, Expression.h, Constraint.h, Solver.h, and SearchNode.h, to get a sense of how these classes interact.

### custom domain_t data structure

The domain_t data structure is defined in types.h. You can choose between GACRangeDomain, BCRangeDomain, and set<int> already. If you want to make your own data structure, just provide a class with methods similar to set<int>, and substitute it into domain_t in types.h.
  
### custom Expression

Create new .h and .cpp files in the folders for Expressions. Override and implement the build, evaluate, and evaluateFake methods. If your expression has special normalization rules, override normalize. If there's a more efficient way to calculate the domain of your expression, you can override getDomain and getInitialDomain to implement it.

### custom Constraint

Create new .h and .cpp files in the folders for Constraints. Override and implement build and isSatisfied. Override and implement propagate - most constraints just call SearchNode::defaultPropagate here, but if a special propagation algorithm is needed or there's a more efficient way to propagate it, you can implement that instead. NOTE: this may be especially relevant for something like a global constraint. Finally, if your constraint needs special normalization rules, also override and implement that.

### custom SearchNode

The SearchNode handles solving each instantaneous CSP, so if you want to use a consistency algorithm (other than Bounds Consistency or Generalized Arc Consistency, which are implemented), you may want to create a new derived class of SearchNode.

To do this, create new .h and .cpp files in the folders for SearchNodes. Override and implement generateNextAssignment and defaultPropagate. NOTE: some classes of constraints may not use the defaultPropagate implementation you provide, using their own custom algorithms instead. Make sure this does not trip up your SearchNode solution algorithm.
