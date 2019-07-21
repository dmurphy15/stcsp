#pragma once

#include <set>
#include <fstream>

class Solver;
class SearchNode;

/***************************** SOLVERPRINTER ***************************************************************************
 * Prints the solution produced by a solver
 * ********************************************************************************************************************/

class SolverPrinter
{
public:
    SolverPrinter() = delete;

    /**
     * Print a text representation of the solution graph to the console
     * @param s - the solver whose solution will be printed
     * @param includeAuxiliaryVariables - whether or not to also print the values of auxiliary variables that the solver
     *  created
     */
    static void printTree(Solver& s, bool includeAuxiliaryVariables=false);
    /**
     * Write the solution graph to a the file solutions.dot
     * @param s - the solver whose solution will be written
     */
    static void writeGraph(Solver& s);

private:

    /**
     * helper for printing the graph
     * @param s - solver whose solution is being printed
     * @param currentState - current SearchNode we are looking at
     * @param visited - set of searchnodes we have already processed (to avoid looping)
     * @param includeAuxiliaryVariables - whether we want to print assignments to auxiliary variables
     * @return visited
     */
    static std::set<SearchNode *>& printTreeRe(Solver& s, SearchNode &currentState, std::set<SearchNode *> &visited, bool includeAuxiliaryVariables);
    /**
     * helper for writing the solution
     * @param s - solver whose solution is being printed
     * @param file - file we are writing to
     * @param currentNode - current node we are looking at
     * @param visited - set of searchnodes we have already processed (to avoid loops)
     */
    static void writeGraphRe(Solver& s, std::ofstream& file, SearchNode &currentNode, std::set<SearchNode *>& visited);
};