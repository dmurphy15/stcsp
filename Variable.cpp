#include "Variable.h"
#include "InstantSolver.h"

Variable::Variable(std::set<int> domain)
{
    mInitialDomain = domain;
}

int Variable::evaluate(InstantSolver &context)
{
    return context.getAssignment(*this);
}

std::set<int> Variable::getInitialDomain() const
{
    return mInitialDomain;
}

std::set<int> Variable::getDomain(InstantSolver &context)
{
    return context.getDomain(*this);
}