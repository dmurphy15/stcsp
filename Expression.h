#pragma once
#include <set>
#include <functional>

class InstantaneousCSP;
class Variable;

/*
 * Interface that all variables and operations on variables must implement
 */
class Expression
{
public:
    Expression(std::set<std::reference_wrapper<Expression>> expressions);

    virtual int evaluate(InstantaneousCSP &context) const = 0;
    virtual std::set<std::reference_wrapper<Variable>> getVariables() const = 0;

    // TODO give it an optional "binarize" function

    // for dominance detection
    friend bool operator< (const Expression &lhs, const Expression &rhs);
    friend bool operator== (const Expression &lhs, const Expression &rhs);
private:
    // the operators will just call these virtual functions. making them virtual so that
    // we can have a default comparison for expressions, but things like variables and constants
    // can override it
    virtual bool lt(const Expression &rhs) const;
    virtual bool eq(const Expression &rhs) const;
    // this is just used for dominance detection; should not be used by derived classes
    std::set<std::reference_wrapper<Expression>> mExpressions;
};