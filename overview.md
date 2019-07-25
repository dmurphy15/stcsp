# Overiew

## The Problem
A stream constraint program

* Definition at http://www.cse.cuhk.edu.hk/~jlee/publ/14/cp14stCSP.pdf
* Extension at http://www.cse.cuhk.edu.hk/~jlee/publ/18/cp18UntilStCSP.pdf

Files in the test folder show some example inputs

* Define your variables
* Specify constraints over those variables
  * If a variable is completely unconstrained, the solver will drop it

## The Solution
To get a solution, your constraints are passed to a Solver. The Solver will output solutions in the form of a Büchi automaton
* In our case, a graph of SearchNodes, where each SearchNode represents a new time step, leading from an initial, root node.
* Each SearchNode is connected to its children through solutions (assignments of values to variables) to the instantaneous CSP contained within the SearchNode.

## Solver
The Solver class handles constructing the solution graph. It performs initialization, graph construction, and graph pruning, at which point it will have a valid solution.

Users should specify the type of SearchNode they want it to use, as well as a prefixK value for those SearchNodes (explained under SearchNode section)

### Initialization
1. Takes in your set of constraints
2. Performs a normalization procedure to obtain a new set of constraints
  * Reduces redundant constraints/expressions
  * May generate additional variables and new constraints/expressions
    * Laid out at http://www.cse.cuhk.edu.hk/~jlee/publ/14/cp14stCSP.pdf and http://www.cse.cuhk.edu.hk/~jlee/publ/18/cp18UntilStCSP.pdf

### Graph Construction
1. Create an initial, root SearchNode using the normalized constraints, and let that be the current node
2. Add the current node to the solution graph
3. Get the next solution for the current node (if there is none, go to step 8)
4. Carry constraints:
  1. If the current node is the root:
    1. Substitute the constraint set for a new constraint set, in which all FirstExpressions have been converted to ConstantExpressions with equivalent values
    2. Check for tautologies (constraints that operate purely on constants) and remove them
  2. Add/update/delete temporal constraints as needed to produce the correct CSP for the next timepoint
5. Create a new SearchNode using the new constraint set
6. Dominance detection
  1. If a node equivalent to the new SearchNode already exists in the solution graph, make the current node (the node that led to the new SearchNode) a parent of the equivalent node in the graph, and return
  2. Otherwise, let the new node be the current node, and repeat recursively from step 2
7. Repeat from step 3
8. If the current node has no child SearchNodes at this point, then it is a failure node, so remove it from the graph

### Graph Pruning
The Solver uses the SolverPruner class to handle this


## SolverPruner
The SolverPruner class prunes the solution graph after it has been constructed. This is necessary only because an UntilConstraint is required to be satisfied 
eventually, so any SearchNodes that have no paths that lead to the satisfaction of all UntilConstraints are therefore failures, and should be cut out of the graph.

## SearchNode
A SearchNode represents an instantaneous CSP (very similar to a traditional CSP). Its only responsibility is to take a set of constraints over
variables and produce assignments of values to those variables that satisfy the constraints.

Solving a CSP is usually made faster by some kind of consistency algorithm. Bounds Consistency and Generalized Arc Consistency versions are already
implemented, but users can implement their own. 

**NOTE:** Certain constraints may have their own special propagation procedures (global constraints, PrimitiveUntilConstraints, PrimitiveNextConstraints, etc.)
which will override the default propagation procedure given by the SearchNode. They currently use Bounds Consistency style algorithms,
though Generalized Arc Consistency implementations are commented out in the src files.

Users are free to further implement their own heuristics for the order of variable selection and value selection.

### prefixK
A SearchNode's prefixK value describes how many future timepoints it will consider when employing its consistency algorithm. For example,
a prefixK of 1 means that it will consider a single timepoint (treating the CSP as a regular CSP). A prefixK of 2 means that when it considers
an assignment to variables, it will try to make sure that the assignment is consistent in the current timepoint, and that it will also not
make the next timepoint inconsistent. A prefixK of 3 will ensure that the next 2 timepoints will not be inconsistent, etc.

**NOTE:** It is difficult to correctly ensure consistency with future timepoints when dealing with FirstExpressions - for this reason,
the current SearchNode implementations only propagate over 1 timepoint in the presence of FirstExpressions.

## Constraint
A Constraint operates on a list of Expressions. It must provide a way to check whether it
is satisfied. It may supply its own normalization rules and propagation algorithm. 

## Expression
An Expression usually operates on a list of sub-Expressions. It must provide a way to evaluate it and get its value. It
may supply its own normalization rules.

### Special Expressions
Some special expressions to note:
* **VariableExpression:** wraps around a single Variable
* **ConstantExpression:** contains no variables; has a constant value
* **FirstExpression:** contains a single sub-Expression, but evaluates to the first value that the sub-Expression takes. The Solver must
be aware of this and treat is specially

## Variable
Represents a variable. Contains a domain of possible values it can take.

## Domain
Data structure representing a sorted set of integers. A few implementations are provided: a set of integers, a single pair representing a 
contiguous range of integers, or a set of pairs representing ranges.
