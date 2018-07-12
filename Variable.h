#pragma once
#include <vector>
#include <unordered_set>
#include <experimental/optional>

class Constraint;

class Variable
{
public:
    Variable(std::unordered_set<int> domain);
    /* will throw an error if the specified time has not been assigned a value */
    int evaluate(int time) const;
    std::unordered_set<int> getDomain(int time) const;
    std::unordered_set<Constraint *> getConstraints() const;
    void addConstraint(Constraint *constraint);
    void removeConstraint(Constraint *constraint);
    /* this will be set by the solver */
    static int prefixK;
private:
    std::unordered_set<Constraint *> mConstraints;
    std::vector<std::unordered_set<int>> mDomains;
    std::vector<std::experimental::optional<int>> mAssignments;
};