#pragma once
#include <vector>
#include <map>
#include <functional>


class Constraint;
class InstantaneousCSP;

class Variable
{
public:
    Variable(std::vector<int> domain);
    int evaluate(InstantaneousCSP &context) const;
    std::vector<int> getDomain(InstantaneousCSP &context) const;
    void setDomain(std::vector<int> domain, InstantaneousCSP &context) const;
    std::vector<std::reference_wrapper<Constraint>> getConstraints(InstantaneousCSP &context) const;
    std::vector<int> getInitialDomain() const;

    friend bool operator< (const Variable &lhs, const Variable &rhs);
    friend bool operator== (const Variable &lhs, const Variable &rhs);
private:
    std::vector<int> mInitialDomain;
};