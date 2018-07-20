#pragma once
#include <unordered_set>

class Variable;

/*
 * Interface that all variables and operations on variables must implement
 */
class Expression
{
public:
    virtual int evaluate(int time) const = 0;
    virtual std::unordered_set<Variable *> getVariables() const = 0;

    // TODO give it an optional "binarize" function
};