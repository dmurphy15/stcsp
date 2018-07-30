#pragma once
#include <vector>
#include <map>
#include <functional>

class InstantSolver;
class Variable;
using Variable_r = std::reference_wrapper<Variable>;

/**
 * will be passed around via pointers
 */
class Variable
{
public:
    Variable(std::vector<int> domain);
    int evaluate(InstantSolver &context);
    std::vector<int> getInitialDomain() const;

    friend bool operator< (const Variable &lhs, const Variable &rhs);
    friend bool operator== (const Variable &lhs, const Variable &rhs);
private:
    std::vector<int> mInitialDomain;
};
