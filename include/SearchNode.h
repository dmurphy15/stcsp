#pragma once

#include <map>
#include <set>
#include <vector>

#include "types.h"

/***************************** SEARCHNODE ******************************************************************************
 * Models a single instantaneous CSP, and allows us to extract all solutions for that instant. Subclasses of this
 * implement different algorithms for solving the CSP.
 * These algorithms may prune the domains of variables using consistency algorithms like Bounds Consistency or
 * Generalized Arc Consistency.
 * Note that pruning domains for variables inside FirstExpressions is difficult to do when prefixK is > 1; the SearchNode
 * implementations (and any constraint with a custom propagation algorithm that may contain FirstExpressions)
 * may want to only prune as if prefixK were 1 (see the defaultPropagate implementations in BCSearchNode or GACSearchNode
 * ********************************************************************************************************************/

class SearchNode
{
public:
    /**
     * Produces a new searchNode, representing a CSP contained within a certain time period
     * (the number of timepoints involved is determined by prefix-k)
     * @param id - the id for this SearchNode
     * @param constraints - the set of constraints involved
     * @param historicalValues - a partial assignment, representing the assigned values of any variables in
     * the previous timepoint that were involved in "primitive next constraints" and which will therefore affect
     * the domains in the current set of timepoints
     * @param domains - the domains that the variables will start with when we begin solving the first timepoint
     * @param constraintSetId - an ID to use as a shortcut to distinguish this set of constraints from different sets.
     * If it's -1, we look up an ID for this set from amongst other sets we've seen (using the SetRegistry static class)
     */
    SearchNode(int id,
                const std::set<Constraint_r>& constraints,
               const assignment_t& historicalValues,
               const std::vector<std::map<Variable_r, domain_t>>& domains,
               int constraintSetId=-1);

    /**
     * get whatever value is currently assigned to a variable at a certain time within this time period
     * @param v - the variable being checked
     * @param time - the time; representing an index in the mAssignments entry for v
     * @return - the value of v at time
     */
    int getAssignment(Variable &v, int time);

    /**
     * set assignment for v to value at time
     * @param v - variable to assign
     * @param time - time at which to assign
     * @param value - value to assign
     */
    void setAssignment(Variable &v, int time, int value);
    void setAssignments(assignment_t& a, int time);

    /**
     * get the domain of v at the specified time index within this SearchNode's time period
     * @param v - the variable being checked
     * @param time - the time at which to check it
     * @return - the domain of v at time
     */
    const domain_t& getDomain(Variable &v, int time) const;

    /**
     * set the domain of v at the specified time index within this SearchNode's time period
     * @param v - the variable whose domain is being set
     * @param domain - the domain to give to v
     * @param time - the time at which to set it
     */
    void setDomain(Variable &v, domain_t domain, int time);

    /**
     * removes a single value from a certain variable's domain
     * @param v - variable whose domain will be pruned
     * @param toPrune - value to remove
     * @param time - time of the domain to prune
     */
    domain_t::const_iterator pruneDomain(Variable &v, domain_t::const_iterator &toPrune, int time);

    /**
     * get all the domains at a certain time
     * @param time - the time for which to get them
     * @return all the domains at time
     */
    const std::map<Variable_r, domain_t> &getDomains(int time) const;

    /**
     * get the set of constraints that constrain this SearchNode
     * @return the set of constraints
     */
    std::set<Constraint_r> getConstraints();

    /**
     * returns a coroutine that can be used to iterate through all valid assignments for this SearchNode
     * @return the coroutine
     */
    coro_assignment_t::pull_type generateNextAssignmentIterator();

    /**
     * adds a new SearchNode as a child of this one, associated with the assignment that takes us from this SearchNode
     * to the next
     * @param child - the child search node
     * @param fullAssignment - the assignment that takes us there
     */
    void addChildNode(SearchNode* child, assignment_t fullAssignment);
    void addParentNode(SearchNode* parent);

    void removeChildNode(SearchNode* child);
    void removeParentNode(SearchNode* parent);
    /**
     * get the map of SearchNodes to vectors of full assignments that can come out of this SearchNode
     * @return the vector
     */
    std::map<SearchNode *, std::vector<assignment_t>> getChildNodes();
    std::set<SearchNode *> getParentNodes();

    /**
     * For a generic constraint, c, provide a default algorithm for propagating c over the domains of variables
     * it constrains, to help prune their domains. Examples are Bounds Consistency and Generalized Arc Consistency
     * @param c - the constraint to enforce
     * @return a map of variables to sets of ints representing those values in the domain of of each variable that were
     * removed
     */
    virtual std::map<Variable_r, std::vector<std::set<int>>> defaultPropagate(Constraint &c) = 0;

    friend bool operator==(SearchNode &lhs, SearchNode &rhs);
    friend bool operator<(SearchNode &lhs, SearchNode &rhs);
    friend class std::hash<SearchNode_r>;

    /** returns how many timepoints this SearchNode will consider when producing solutions */
    int getPrefixK() const;
    /** id that uniquely specifies this SearchNode */
    const int id;
    /** get the id for the set of constraints this SearchNode is using */
    int getConstraintSetId() { return mConstraintSetId; };
    /** special id for the root SearchNode */
    const static int ROOT_ID;
protected:
    /**
     * helper function from which one could create a coroutine to iterate through all valid output assignments
     * @param yield - the parent routine that will receive the assignments that this produces
     */
    virtual void generateNextAssignment(coro_assignment_t::push_type &yield) = 0;

    /**
     * vector of assignments to the variables; this enables us to evaluate Expressions within the context of this
     * particular SearchNode
     */
    std::vector<assignment_t> mAssignments;
    /**
     * maps the variables to their domains at each time point
     */
    std::vector<std::map<Variable_r, domain_t>> mDomains;
    /** constraints for this CSP */
    std::set<Constraint_r> mConstraints;
    /** historical values that constrain the domains of certain variables at time 0 from the start */
    assignment_t mHistoricalValues;
    /** maps a child searchnode to a vector of assignments that lead from this searchnode to that child
     * we can use pointers here, because the solver keeps a set of seen searchnodes, which will eliminate duplicates */
    std::map<SearchNode *, std::vector<assignment_t>> mChildNodes; // using a vector bc we do care when different assignments are used to reach the same child
    std::set<SearchNode *> mParentNodes; // using a set bc we don't care when the same parent is added multiple times
private:
    int mConstraintSetId;
};

namespace std {
    template <>
    struct hash<SearchNode_r>
    {
        // hashing based solely on mHistoricalValues, since I'm expecting mConstraints to be pretty
        // similar between timepoints, so if there are collisions I'm guessing there won't be many
        std::size_t operator()(const SearchNode& s) const
        {
            std::size_t seed = 0;
            for (auto pair : s.mHistoricalValues) {
                seed ^= (std::size_t)&(pair.first.get()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                seed ^= pair.second + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
}