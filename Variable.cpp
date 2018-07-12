#include "Variable.h"

Variable::Variable(std::unordered_set<int> domain)
{
    for (int i=0;i<prefixK;i++)
    {
        mDomains.push_back(domain);
        mAssignments.push_back(std::experimental::nullopt);
    }
}

int Variable::evaluate(int time) const
{
    return mAssignments[time].value();
}

std::unordered_set<int> Variable::getDomain(int time) const
{
    return mDomains[time];
}

std::unordered_set<Constraint *> Variable::getConstraints() const
{
    return mConstraints;
}

void Variable::addConstraint(Constraint *constraint)
{
    mConstraints.insert(constraint);
}

void Variable::removeConstraint(Constraint *constraint)
{
    mConstraints.erase(constraint);
}

int Variable::prefixK = -1;