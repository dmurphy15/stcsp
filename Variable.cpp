#include "Variable.h"
#include "InstantaneousCSP.h"

Variable::Variable(std::vector<int> domain)
{
    mInitialDomain = domain;
}

int Variable::evaluate(InstantaneousCSP *context)
{
    return context->assignments[this];
}

std::vector<int> Variable::getDomain(InstantaneousCSP *context)
{
    return context->domains[this];
}

void Variable::setDomain(std::vector<int> domain, InstantaneousCSP *context)
{
    context->domains.insert({this, domain});
}

std::vector<Constraint *> Variable::getConstraints(InstantaneousCSP *context)
{
    return context->variableToConstraints[this];
}

std::vector<int> Variable::getInitialDomain() const
{
    return mInitialDomain;
}