#pragma once
#include <vector>
#include <map>
#include <functional>


class Constraint;
class InstantaneousCSP;
class Variable;
using Variable_r = std::reference_wrapper<Variable>;

/**
 * will be passed around via pointers
 */
class Variable
{
    using Constraint_r = std::reference_wrapper<Constraint>;
public:
    Variable(std::vector<int> domain);
    Variable(const Variable&) = delete;
    Variable& operator=(const Variable&) = delete;
    int evaluate(InstantaneousCSP &context);
    std::vector<int> getDomain(InstantaneousCSP &context);
    void setDomain(std::vector<int> domain, InstantaneousCSP &context);
    std::vector<Constraint_r> getConstraints(InstantaneousCSP &context);
    std::vector<int> getInitialDomain() const;

    friend bool operator< (const Variable &lhs, const Variable &rhs);
    friend bool operator== (const Variable &lhs, const Variable &rhs);
private:
    std::vector<int> mInitialDomain;
};
