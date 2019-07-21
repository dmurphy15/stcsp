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

/***************************** SearchNodeFactory **************************************************************************
 * Creates new SearchNodes for the solver
 * ********************************************************************************************************************/

class SearchNodeFactory
{
public:
    SearchNodeFactory() = delete;
    /**
     * Create a new SearchNode
     * @param SearchNodeType - what type of SearchNode to produce
     * @param constraints - the constraints for the searchnode
     * @param historicalValues - the historical values for the searchnode
     * @param domains - domains of the variables within the searchnode
     * @param constraintSetId - constraintSetId for the searchnode
     * @return a new searchnode built using the parameters
     */
    static SearchNode& MakeSearchNode(SearchNodeType SearchNodeType,
                                      const std::set<Constraint_r>& constraints,
                                      const assignment_t& historicalValues,
                                      const std::vector<std::map<Variable_r, domain_t>>& domains,
                                      int constraintSetId=-1) {
        switch (SearchNodeType) {
            case GAC_NODE :
                return *new GACSearchNode(constraints, historicalValues, domains, constraintSetId);
            case BC_NODE :
                return *new BCSearchNode(constraints, historicalValues, domains, constraintSetId);
            default :
                throw std::invalid_argument("use a valid instant solver name pls. thx.\n");
        }
    }
};