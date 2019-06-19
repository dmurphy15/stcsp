#include "../include/SearchNode.h"

#include "../include/SetRegistry.h"
#include "../include/Variable.h"
#include "../include/Constraint.h"

#include <iostream>
SearchNode::SearchNode(const std::set<Constraint_r>& constraints,
                       const assignment_t& historicalValues,
                       const std::vector<std::pair<std::map<Variable_r, domain_t>::const_iterator,
                                                   std::map<Variable_r, domain_t>::const_iterator>>& domains,
                       int constraintSetId)
        : id(idSource++) {
    mConstraints = constraints;
    mHistoricalValues = historicalValues;
    for (auto&& v : domains) {
        mDomains.push_back({v.first, v.second});
    }
    for (auto &assignment : historicalValues) {
        mDomains[0][assignment.first] = {assignment.second};
    }
    if (id==SearchNode::ROOT_ID) {
        SearchNode::root = this;
    }
    if (constraintSetId < 0) {
        mConstraintSetId = SetRegistry::GetConstraintSetId(mConstraints);
    } else {
        mConstraintSetId = constraintSetId;
    }
    mAssignments.resize(getPrefixK());
}

int SearchNode::getAssignment(Variable &v, int time) {
    return mAssignments.at(time).at(v);
}
void SearchNode::setAssignment(Variable &v, int time, int value) {
    mAssignments.at(time)[v] = value;
}
void SearchNode::setAssignments(assignment_t &a, int time) {
    mAssignments.at(time) = a;
}
const domain_t& SearchNode::getDomain(Variable &v, int time) const {
    return mDomains.at(time).at(v);
}
void SearchNode::setDomain(Variable &v, domain_t domain, int time) {
    mDomains.at(time).at(v) = domain;
}
domain_t::const_iterator SearchNode::pruneDomain(Variable &v, domain_t::const_iterator &toPrune, int time) {
    return mDomains.at(time).at(v).erase(toPrune);
}

const std::map<Variable_r, domain_t>& SearchNode::getDomains(int time) const {
    return mDomains.at(time);
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
void SearchNode::addParentNode(SearchNode &parent) {
    mParentNodes.insert(parent);
}
void SearchNode::removeChildNode(SearchNode &child) {
    for (auto it = mChildNodes.begin(); it != mChildNodes.end(); it++) {
        SearchNode& s = it->first;
        if (&s == &child) {
            mChildNodes.erase(it);
            return;
        }
    }
}
void SearchNode::removeLastChildNode() {
    mChildNodes.pop_back();
}
void SearchNode::removeParentNode(SearchNode& p) {
    for (auto it = mParentNodes.begin(); it != mParentNodes.end(); it++) {
        if (&(it->get()) == &p) {
            mParentNodes.erase(it);
        }
    }
}
std::vector<std::pair<SearchNode_r, assignment_t>> SearchNode::getChildNodes() {
    return mChildNodes;
}
std::set<SearchNode_r> SearchNode::getParentNodes() {
    return mParentNodes;
}

int SearchNode::getPrefixK() const {
    return (int)mDomains.size();
}

bool operator==(SearchNode &lhs, SearchNode &rhs) {
    return (&lhs == &rhs) ||
           ((lhs.mConstraintSetId == rhs.mConstraintSetId) &&
            (lhs.mHistoricalValues == rhs.mHistoricalValues));
//    return (&lhs == &rhs) || ((lhs.mHistoricalValues == rhs.mHistoricalValues) &&
//            ((lhs.constraintSetId == rhs.constraintSetId) || (lhs.mConstraints == rhs.mConstraints)));

//    return (lhs.mHistoricalValues == rhs.mHistoricalValues) &&
//            (lhs.mConstraints == rhs.mConstraints);
}

bool operator<(SearchNode &lhs, SearchNode &rhs) {
    return (lhs.mConstraintSetId < rhs.mConstraintSetId) ||
           (lhs.mConstraintSetId == rhs.mConstraintSetId && lhs.mHistoricalValues < rhs.mHistoricalValues);

//    return ((&lhs != &rhs) && (lhs.constraintSetId != rhs.constraintSetId)) &&
//            ((lhs.mHistoricalValues < rhs.mHistoricalValues)
//           || ((lhs.mHistoricalValues == rhs.mHistoricalValues)
//               && (lhs.mConstraints < rhs.mConstraints)));
}

const int SearchNode::ROOT_ID = 0;
int SearchNode::idSource = SearchNode::ROOT_ID;
SearchNode *SearchNode::root = nullptr;

coro_assignment_t::pull_type SearchNode::generateNextAssignmentIterator() {
    return coro_assignment_t::pull_type(boost::bind(&SearchNode::generateNextAssignment, this, _1));
}