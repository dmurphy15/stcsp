#include "Variable.h"
#include "SearchNode.h"

Variable::Variable(std::set<int> domain)
{
    mInitialDomain = domain;
}

int Variable::evaluate(SearchNode &context, int time)
{
    return context.getAssignment(*this, time);
}

std::set<int> Variable::getInitialDomain() const
{
    return mInitialDomain;
}

std::set<int> Variable::getDomain(SearchNode &context, int time)
{
    return context.getDomain(*this, time);
}