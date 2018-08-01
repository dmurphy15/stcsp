#include "InstantSolver.h"

#include "Variable.h"
#include "Constraint.h"


InstantSolver::InstantSolver(std::set<Constraint_r> constraints, std::map<Variable_r, int> inputAssignments) {
    mConstraints = constraints;
    mInputAssignments = inputAssignments;
}

int InstantSolver::getAssignment(Variable &v) {
    return mAssignments.at(v);
}
std::set<int> InstantSolver::getDomain(Variable &v) {
    return mDomains.at(v);
}
void InstantSolver::setDomain(Variable &v, std::set<int> domain) {
    mDomains.at(v) = domain;
}

std::set<Constraint_r> InstantSolver::getConstraints() {
    return mConstraints;
}


//this lets the solver set individual child nodes. so the solver will get a statenode from the iterator,
//then look for previous equivalent statenodes. If it finds a previous one, it will add that as the
//child and get the next state node etc, if it does not, then it adds the new one as the child and
//recurs on it, before getting the next state node
void InstantSolver::addChildNode(InstantSolver &child, std::map<Variable_r, int> assignment) {
    mChildNodes.push_back({child, assignment});
}
void InstantSolver::removeLastChildNode() {
    mChildNodes.pop_back();
}
std::vector<std::pair<InstantSolver_r, std::map<Variable_r, int>>> InstantSolver::getChildNodes() {
    return mChildNodes;
}


bool operator==(InstantSolver &lhs, InstantSolver &rhs) {
    return (lhs.mInputAssignments == rhs.mInputAssignments) && (lhs.mConstraints == rhs.mConstraints);
}