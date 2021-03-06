#pragma once

#include <vector>
#include <set>
#include <map>
#include <functional>
#include <memory>

#include "types.h"
#include "SearchNode.h"

enum SearchNodeType : int;

/***************************** SOLVER *********************************************************************************
 * Finds the solution to a given stream-CSP. Does so by constructing a graph of SearchNodes, iterating through each
 * solution to each SearchNode and determining whether it leads to a valid final solution.
 * ********************************************************************************************************************/

class Solver
{
public:
    /**
     * Creates a solver that will use SearchNodes to solve a given stream-CSP
     * @param searchNodeType - enum to indicate what type of SearchNode you want to use to obtain solutions for each
     *      instantaneous timepoint, defined in SearchNodeTypes.h
     * @param prefixK - how many timepoints into the future to you want each produced SearchNode to consider when
     *      propagating constraints to eliminate impossible assignments to variables
     * @param constraints - a set of constraints that represent the stream-CSP
     */
    Solver(SearchNodeType searchNodeType, int prefixK, const std::set<Constraint_r>& constraints);
    /** create a solver with an empty set of constraints to start */
    Solver(SearchNodeType searchNodeType, int prefixK);
    /** add a constraint to the solver's set of constraints */
    void addConstraint(Constraint &c);
    /**
     * Solve the stream-CSP defined by the solver's set of constraints
     */
    void solve();
    /**
     * Print a representation of the final solution state graph to stdout
     */
    void printTree();
    /**
     * Write the final solution state graph to a .dot file
     *      (helpful linux command to convert .dot to .png:
     *          sfdp -x -Goverlap=scale -Tpng data.dot > data.png
     *      )
     */
    void writeGraph();
    /**
     * Get a pointer to the solution graph directly
     */
    std::shared_ptr<SearchNode> getSolutionGraph() { return mTree; }
private:
    /** what kind of SearchNode will we use */
    SearchNodeType mNodeType;
    /** the root SearchNode in the state graph that the solver produces to solve the stream-CSP */
    std::shared_ptr<SearchNode> mTree;

    /**
     * Iterate through all the valid assignments in the current timepoint and recursively iterate through all
     * valid assignments of the resulting states at the following timepoints, to construct the final solution
     * state graph.
     * Does dominance detection and uses carryConstraints to set up the next searchNode
     * @param currentNode - the SeachNode whose solutions we are iterating through at the current timepoint
     * @return false if currentState is a failure node (all of its lines of descendant nodes eventually reach a state
     *      with no solution, without creating cycles in the state graph), and true otherwise
     */
    bool solveRe(SearchNode &currentNode);

    /**
     * Given the a set of constraints that define the current SearchNode and an assignment to variables representing
     * a solution for this SearchNode, create a set of constraints to define the next SearchNode and a set of forced
     * assignments to variables in the next SearchNode. Uses basic tautology detection.
     * Also freezes the values of FirstExpressions after we have solved the first searchNode.
     * @param constraints - an input set of constraints that define the current SearchNode
     * @param assignment - an input assignment to variables, representing a solution to the current node
     * @param carriedConstraints - an output set of constraints for the child SearchNode
     * @param carriedAssignments - an output set of forced assignments to variables for the child SearchNode
     * @param solvingFirstNode - true if we are solving the root node, else false (to help tautology detection)
     * @return true if the output set of constraints differs from the input set of constraints; false else
     */
    bool carryConstraints(SearchNode& currentNode,
                            std::set<Constraint_r>& carriedConstraints,
                          assignment_t& carriedAssignments);
    /**
     * Take a set of constraints and return a set of constraints that are equivalent, except that all
     * FirstExpressions have been replaced with ConstantExpressions whose values equal the values that the
     * FirstExpressions took
     * @param constraints - the set of unfrozen constraints
     * @return a set of constraints where the firstExpressions have been replaced
     */
    std::set<Constraint_r> freezeFirstExpressions(SearchNode& rootNode);
    /** the original variables and constraints that defined the problem, before any refactoring by the solver */
    std::set<Variable_r> mOriginalVariables;
    std::set<Constraint_r> mOriginalConstraints;
    /** a complete set of variables, after normalizing the program */
    std::set<Variable_r> mVariables;

    /** maps SearchNodes to a bool indicating whether they were non-failure nodes; used for dominance detection */
    std::unordered_map<SearchNode_r, bool> mSeenSearchNodes;

    /** map of variables to their full domains, for efficient initialization */
    std::map<Variable_r, domain_t> mDomainsInitializer;
    /** how many timepoints should each SearchNode consider during constraint propagation */
    int mPrefixK;

    int mSearchNodeIdSource = SearchNode::ROOT_ID;

    /** convenience class for printing solutions */
    friend class SolverPrinter;
};