#include "../include/Variable.h"
#include "../include/SearchNode.h"

Variable::Variable(const domain_t& domain)
{
    mInitialDomain = domain;
}

int Variable::evaluate(SearchNode &context, int time)
{
    return context.getAssignment(*this, time);
}

const domain_t& Variable::getInitialDomain() const
{
    return mInitialDomain;
}

const domain_t& Variable::getDomain(SearchNode &context, int time)
{
    return context.getDomain(*this, time);
}