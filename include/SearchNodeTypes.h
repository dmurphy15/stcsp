#pragma once

/***************************** SearchNodeType **************************************************************************
 * Create a searchnodetype for each type of searchnode you implement. This is used by the SearchNodeFactory in order
 * to determine the type of searchnode to produce.
 * ********************************************************************************************************************/

enum SearchNodeType : int {
    GAC_NODE,
    BC_NODE,
};