#include "SearchNode.h"

#include "Variable.h"
#include "Constraint.h"


SearchNode::SearchNode(std::set<Constraint_r> constraints, assignment_t historicalValues, std::vector<std::map<Variable_r, domain_t>> domains) {
    mConstraints = constraints;
    mHistoricalValues = historicalValues;
    mDomains = domains;
}

int SearchNode::getAssignment(Variable &v, int time) {
    return mAssignments[time].at(v);
}
void SearchNode::setAssignment(Variable &v, int time, int value) {
    mAssignments[time].at(v) = value;
}
const domain_t& SearchNode::getDomain(Variable &v, int time) const {
    return mDomains[time].at(v);
}
void SearchNode::setDomain(Variable &v, domain_t domain, int time) {
    mDomains[time].at(v) = domain;
}
domain_t::iterator SearchNode::pruneDomain(Variable &v, domain_t::iterator &toPrune, int time) {
    return mDomains[time].at(v).erase(toPrune);
}

const std::map<Variable_r, domain_t>& SearchNode::getDomains(int time) const {
    return mDomains[time];
}

std::set<Constraint_r> SearchNode::getConstraints() {
    return mConstraints;
}


//this lets the solver set individual child nodes. so the solver will get a statenode from the iterator,
//then look for previous equivalent statenodes. If it finds a previous one, it will add that as the
//child and get the next state node etc, if it does not, then it adds the new one as the child and
//recurs on it, before getting the next state node
void SearchNode::addChildNode(SearchNode &child, assignment_t assignment) {
    mChildNodes.push_back({child, assignment});
}
void SearchNode::removeLastChildNode() {
    mChildNodes.pop_back();
}
std::vector<std::pair<SearchNode_r, assignment_t>> SearchNode::getChildNodes() {
    return mChildNodes;
}

int SearchNode::getPrefixK() {
    return mDomains.size();
}

bool operator==(SearchNode &lhs, SearchNode &rhs) {
    return (lhs.mHistoricalValues == rhs.mHistoricalValues) && (lhs.mConstraints == rhs.mConstraints);
}

bool operator<(SearchNode &lhs, SearchNode &rhs) {
    return (lhs.mHistoricalValues < rhs.mHistoricalValues)
            || ((lhs.mHistoricalValues == rhs.mHistoricalValues)
                && (lhs.mConstraints < rhs.mConstraints));
}