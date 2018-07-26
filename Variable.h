#pragma once
#include <vector>
#include <map>
#include <functional>


class Constraint;
class InstantaneousCSP;

/**
 * will be passed around via pointers
 */
class Variable
{
public:
    Variable(std::vector<int> domain);
    int evaluate(InstantaneousCSP *context);
    std::vector<int> getDomain(InstantaneousCSP *context);
    void setDomain(std::vector<int> domain, InstantaneousCSP *context);
    std::vector<Constraint *> getConstraints(InstantaneousCSP *context);
    std::vector<int> getInitialDomain() const;

private:
    std::vector<int> mInitialDomain;
};