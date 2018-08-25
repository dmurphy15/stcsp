#pragma once

#include <set>
#include <vector>
#include <map>
#include <functional>
#include <stdexcept>

#include "types.h"
#include "SearchNodeTypes.h"
#include "searchNodes/GACSearchNode.h"
#include "searchNodes/BCSearchNode.h"

class SearchNodeFactory
{
public:
    SearchNodeFactory() = delete;
    static SearchNode& MakeSearchNode(SearchNodeType SearchNodeType,
                                      const std::set<Constraint_r>& constraints,
                                      const assignment_t& historicalValues,
                                      const std::vector<std::map<Variable_r, domain_t>>& domains) {
        switch (SearchNodeType) {
            case GAC_NODE :
                return *new GACSearchNode(constraints, historicalValues, domains);
            case BC_NODE :
                return *new BCSearchNode(constraints, historicalValues, domains);
            default :
                throw std::invalid_argument("use a valid instant solver name pls. thx.\n");
        }
    }
};