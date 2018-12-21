#include "../include/Variable.h"
#include "../include/SearchNode.h"

Variable::Variable(const Domain& domain, const char *name) : mName(name)
{
    mInitialDomain = domain;
}

int Variable::evaluate(SearchNode &context, int time)
{
    return context.getAssignment(*this, time);
}

const Domain& Variable::getInitialDomain() const
{
    return mInitialDomain;
}

const Domain& Variable::getDomain(SearchNode &context, int time)
{
    return context.getDomain(*this, time);
}