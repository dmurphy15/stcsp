#include "../include/SolverPrinter.h"

#include <iostream>
#include <fstream>

#include "../include/Solver.h"
#include "../include/SearchNode.h"
#include "../include/Variable.h"

void SolverPrinter::printTree(Solver& s, bool includeAuxiliaryVariables) {
    std::cout<<s.mOriginalVariables.size();
    std::set<SearchNode *> visited;
    printTreeRe(s, *s.mTree, visited, includeAuxiliaryVariables);
    std::cout<<"\n";
}

std::set<SearchNode *>& SolverPrinter::printTreeRe(Solver& s, SearchNode &currentState, std::set<SearchNode *> &visited, bool includeAuxiliaryVariables) {
    if (visited.find(&currentState) != visited.end()) {
        return visited;
    }
    std::cout<<"state "<<currentState.id<<":\n";
    for (auto &pair : currentState.getChildNodes()) {
        SearchNode* child = pair.first;
        for (auto &assignment : pair.second) {
            std::cout<<"\tchild "<<child->id<<" with assignments:\n";
            for (auto &p : assignment) {
                Variable &v = p.first;
                if (s.mOriginalVariables.find(v) != s.mOriginalVariables.end() || includeAuxiliaryVariables) {
                    std::cout<<"\t\tvariable at "<<(v.mName)<<" with value "<<p.second<<"\n";
                }
            }
        }
    }
    visited.insert(&currentState);
    for (auto &pair : currentState.getChildNodes()) {
        SearchNode* child = pair.first;
        visited = printTreeRe(s, *child, visited, includeAuxiliaryVariables);
    }
    return visited;
}

void SolverPrinter::writeGraph(Solver& s) {
    std::ofstream file("solutions2.dot");
    // !!!!!!!!!!! should fix this. mSeenSearchNodes accurately represents the number of searchnodes in the solution
    // before pruning for UntilConstraints, but after pruning, the final solution may contain fewer nodes than this
    file<<"# Number of SearchNodes = "<<s.mSeenSearchNodes.size()<<"\n";
    file<<"# Number of Total Variables = "<<s.mVariables.size()<<"\n";
    file<<"# Number of Named Variables = "<<s.mOriginalVariables.size()<<"\n";
    file<<"# Named Variables:\n";
    file<<"#";
    for (Variable &v : s.mOriginalVariables) {
        file<<" "<<v.mName;
    }
    file<<"\n";
    file<<"digraph \"StCSP\" {\n"
          "graph [K=\"2\"];\n";
    std::set<SearchNode *> visited;
    writeGraphRe(s, file, *s.mTree, visited);
    file<<"}\n";
    file.close();
}

void SolverPrinter::writeGraphRe(Solver& s, std::ofstream& file, SearchNode &currentNode, std::set<SearchNode *> &visited) {
    file<<""<<currentNode.id<<" [shape=circle, label=\""<<currentNode.id<<"\"];\n";
    for (auto pair : currentNode.getChildNodes()) {
        for (auto assignment : pair.second) {
            file<<""<<currentNode.id<<" -> "<<pair.first->id<<" [label=\" ";
            for (auto p : assignment) {
                Variable& v = p.first;
                if (s.mOriginalVariables.find(v) != s.mOriginalVariables.end()) {
                    file<<v.mName<<":"<<p.second<<" ";
                }
            }
            file<<"\"]\n";
        }
    }
    visited.insert(&currentNode);
    for (auto pair : currentNode.getChildNodes()) {
        SearchNode* node = pair.first;
        if (visited.find(node) == visited.end()) {
            writeGraphRe(s, file, *node, visited);
        }
    }
}