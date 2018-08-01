#pragma once
#include <set>
#include <functional>

class InstantSolver;
class Variable;
using Variable_r = std::reference_wrapper<Variable>;
class Expression;
using Expression_r = std::reference_wrapper<Expression>;
class Constraint;
using Constraint_r = std::reference_wrapper<Constraint>;

/*
 * Interface that all variables and operations on variables must implement
 */
class Expression
{
public:
    virtual int evaluate(InstantSolver &context) const = 0;
    virtual std::set<Variable_r> getVariables() const = 0;
    virtual Expression& normalize(std::set<Constraint_r> &constraintList,
                           std::set<Variable_r> &variableList) = 0;
    virtual std::set<int> getDomain(InstantSolver &context) const = 0;
    virtual std::set<int> getInitialDomain() const = 0;

    // TODO give it an optional "binarize" function

//    virtual bool lt(Expression *rhs) const;
//    static struct
    friend bool operator ==(const Expression &a, const Expression &b) {
        return a.eq(b);
    }
    friend bool operator <(const Expression &a, const Expression &b) {
        return a.lt(b);
    }
private:
    // the operators will just call these virtual functions. making them virtual so that
    // we can have a default comparison for expressions, but things like variables and constants
    // can override it
    virtual bool lt(const Expression &rhs) const = 0;
    virtual bool eq(const Expression &rhs) const = 0;
};