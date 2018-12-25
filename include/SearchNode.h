#pragma once

#include <map>
#include <set>
#include <vector>

#include "types.h"
#include "Domain.h"

class SearchNode
{
public:
    /**
     * Produces a new searchNode, representing a CSP contained within a certain time period
     * (the number of timepoints involved is determined by prefix-k)
     * @param constraints - the set of constraints involved
     * @param historicalValues - a partial assignment, representing the assigned values of any variables in
     * @param domains - the domains that the variables will start with when we begin solving the first timepoint
     * the previous timepoint that were involved in "primitive next constraints" and which will therefore affect
     * the domains in the current set of timepoints
     */
    SearchNode(const std::set<Constraint_r>& constraints,
               const assignment_t& historicalValues,
               const std::vector<std::map<Variable_r, Domain>>& domains);

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

    /**
     * get the domain of v at the specified time index within this SearchNode's time period
     * @param v - the variable being checked
     * @param time - the time at which to check it
     * @return - the domain of v at time
     */
    const Domain& getDomain(Variable &v, int time) const;

    /**
     * set the domain of v at the specified time index within this SearchNode's time period
     * @param v - the variable whose domain is being set
     * @param time - the time at which to set it
     * @param domain - the domain to give to v
     */
    void setDomain(Variable &v, Domain domain, int time);

    /**
     * removes a single value from a certain variable's domain
     * @param v - variable whose domain will be pruned
     * @param toPrune - value to remove
     * @param time - time of the domain to prune
     */
    Domain::const_iterator pruneDomain(Variable &v, Domain::const_iterator &toPrune, int time);

    /**
     * get all the domains at a certain time
     * @param time - the time for which to get them
     * @return all the domains at time
     */
    const std::map<Variable_r, Domain> &getDomains(int time) const;

    /**
     * get the set of constraints that constrain this SearchNode
     * @return the set of constraints
     */
    std::set<Constraint_r> getConstraints();

    /**
     * returns a coroutine that can be used to iterate through all valid assignments for this SearchNode
     * @return the coroutine
     */
    virtual coro_assignment_t::pull_type generateNextAssignmentIterator() = 0;

    /**
     * adds a new SearchNode as a child of this one, associated with the assignment that takes us from this SearchNode
     * to the next
     * @param child - the child search node
     * @param fullAssignment - the assignment that takes us there
     */
    void addChildNode(SearchNode &child, assignment_t fullAssignment);
    void addParentNode(SearchNode &parent);

    /**
     * removes the last childNode that was added
     */
    void removeLastChildNode();

    void removeChildNode(SearchNode& child);
    void removeParentNode(SearchNode& parent);
    /**
     * get the vector of pairs of SearchNodes and full assignments that can come out of this SearchNode
     * @return the vector
     */
    std::vector<std::pair<SearchNode_r, assignment_t>> getChildNodes();
    std::set<SearchNode_r> getParentNodes();

    /**
     * reduces the domain of v until the only remaining values are those for which c can be satisfied
     * @param v - the variable whose domain will be reduced
     * @param c - the constraint to enforce
     * @param time - the time index of the domain to reduce
     * @return a set of ints representing those values in the domain of v that were removed
     */
    virtual std::vector<std::set<int>> defaultPropagate(Variable &v, Constraint &c) = 0;

    friend bool operator==(SearchNode &lhs, SearchNode &rhs);
    friend bool operator<(SearchNode &lhs, SearchNode &rhs);
    friend class std::hash<SearchNode_r>;

    int getPrefixK() const;
    const int id;
protected:
    /**
     * helper function from which one could create a coroutine to iterate through all valid output assignments
     * @param yield - the parent routine that will receive the assignments that this produces
     */
    virtual void generateNextAssignment(coro_assignment_t::push_type &yield) = 0;

    /**
     * vector of assignments to the variables
     */
    std::vector<assignment_t> mAssignments;
    /**
     * maps the variables to their domains at each time point
     */
    std::vector<std::map<Variable_r, Domain>> mDomains;
    std::set<Constraint_r> mConstraints;
    assignment_t mHistoricalValues;
    std::vector<std::pair<SearchNode_r, assignment_t>> mChildNodes; // using a vector bc we do care when different assignments are used to reach the same child
    std::set<SearchNode_r> mParentNodes; // using a set bc we don't care when the same parent is added multiple times
private:
    static int idSource;
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