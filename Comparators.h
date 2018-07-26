#pragma once

class Expression;
class Constraint;

struct ExpressionLtComparator {
    // making it virtual so that we can have a default comparison for expressions,
    // but things like variables and constants
    bool operator() (Expression *lhs, Expression *rhs);
};

struct ConstraintLtComparator {
    bool operator() (Constraint *lhs, Constraint *rhs);
};