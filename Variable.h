#pragma once
#include <set>
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
    Variable(std::set<int> domain);
    int evaluate(InstantSolver &context);
    std::set<int> getInitialDomain() const;
    std::set<int> getDomain(InstantSolver &context);

    friend bool operator <(const Variable &a, const Variable &b) {
        return &a < &b;
    }
    friend bool operator ==(const Variable &a, const Variable &b) {
        return &a == &b;
    }
private:
    std::set<int> mInitialDomain;
};
