#pragma once
#include <set>
#include <functional>

class InstantSolver;
class Variable;
class Expression;
using Expression_r = std::reference_wrapper<Expression>;

/*
 * Interface that all variables and operations on variables must implement
 */
class Expression
{
    using Variable_r = std::reference_wrapper<Variable>;

public:
    Expression() = delete;
    Expression(std::set<Expression_r> expressions);

    virtual int evaluate(InstantSolver &context) const = 0;
    virtual std::set<Variable_r> getVariables() const = 0;

    // TODO give it an optional "binarize" function

//    virtual bool lt(Expression *rhs) const;
//    static struct
    friend bool operator< (const Expression &lhs, const Expression &rhs);
    friend bool operator== (const Expression &lhs, const Expression &rhs);
private:
    // the operators will just call these virtual functions. making them virtual so that
    // we can have a default comparison for expressions, but things like variables and constants
    // can override it
    virtual bool lt(const Expression &rhs) const;
    virtual bool eq(const Expression &rhs) const;
    // this is just used for dominance detection; should not be used by derived classes
    std::set<Expression_r> mExpressions;
};