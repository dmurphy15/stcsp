#pragma once

#include <set>
#include <vector>
#include <map>
#include <functional>
#include <stdexcept>

#include "types.h"
#include "searchNodes/GACSearchNode.h"

enum SearchNodeType : int {
    GAC_NODE
};

class SearchNodeFactory
{
public:
    SearchNodeFactory() = delete;
    static SearchNode& MakeSearchNode(SearchNodeType SearchNodeType,
                                      std::set<Constraint_r> constraints,
                                      assignment_t historicalValues,
                                      std::vector<std::map<Variable_r, domain_t>> domains) {
        switch (SearchNodeType) {
            case GAC_NODE :
                return *new GACSearchNode(constraints, historicalValues, domains);
            default :
                throw std::invalid_argument("use a valid instant solver name pls. thx.\n");
        }
    }
};