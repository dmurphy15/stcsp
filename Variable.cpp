#include "Variable.h"
#include "InstantSolver.h"

Variable::Variable(std::vector<int> domain)
{
    mInitialDomain = domain;
}

int Variable::evaluate(InstantSolver &context)
{
    return context.getAssignment(*this);
}

std::vector<int> Variable::getInitialDomain() const
{
    return mInitialDomain;
}

bool operator< (const Variable &lhs, const Variable &rhs)
{
    return &lhs < &rhs;
}

bool operator== (const Variable &lhs, const Variable &rhs)
{
    return &lhs == &rhs;
}