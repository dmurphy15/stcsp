#pragma once

#include <set>
#include <fstream>

class Solver;
class SearchNode;

class SolverPrinter
{
public:
    SolverPrinter() = delete;

    static void printTree(Solver& s, bool includeAuxiliaryVariables=false);
    static void writeGraph(Solver& s);

private:

    static std::set<SearchNode *>& printTreeRe(Solver& s, SearchNode &currentState, std::set<SearchNode *> &visited, bool includeAuxiliaryVariables);
    static void writeGraphRe(Solver& s, std::ofstream& file, SearchNode &currentNode, std::set<SearchNode *>& visited);
};