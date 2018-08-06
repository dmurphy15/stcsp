#include "PrimitiveFirstConstraint.h"

#include <algorithm>

#include "../expressions/VariableExpression.h"
#include "../Variable.h"
#include "../SearchNode.h"

PrimitiveFirstConstraint::PrimitiveFirstConstraint(VariableExpression &variableExpr, Expression &firstExpr) :
        mVariableExpr(variableExpr),
        mFirstExpr(firstExpr),
        mVariable(*variableExpr.getVariables().begin()){}

PrimitiveFirstConstraint::~PrimitiveFirstConstraint() {}

void PrimitiveFirstConstraint::normalize(std::set<Constraint_r> &constraintList,
                                         std::set<Variable_r> &variableList)
{
    throw std::logic_error("this should have only been produced through normalization, so the contents should have already been normalized");
}

int PrimitiveFirstConstraint::isSatisfied(SearchNode &context, int time) const
{
    return mVariableExpr.evaluate(context, time) == mFirstExpr.evaluate(context, 0);
}

std::set<Variable_r> PrimitiveFirstConstraint::getVariables() const
{
    std::set<Variable_r> vars1 = mFirstExpr.getVariables();
    std::set<Variable_r> vars2 = mVariableExpr.getVariables();
    vars1.insert(vars2.begin(), vars2.end());
    return vars1;
}

std::vector<std::set<int>> PrimitiveFirstConstraint::propagate(Variable &v, SearchNode &context)
{
    std::vector<std::set<int>> ret(context.getPrefixK());
    if (v == mVariable) {
        // mVariable should be constant, so we can just propagate for 1 timepoint and duplicate the domain
        std::set<int> firstDifference;
        std::set<Variable_r> othersTmp = mFirstExpr.getVariables();
        std::vector<Variable_r> others(othersTmp.begin(), othersTmp.end());
        for (auto iter = context.getDomain(v, 0).begin(); iter != context.getDomain(v, 0).end(); ) {
            context.setAssignment(v, *iter, 0);
            if (shouldPrune(context, others, 0)) {
                firstDifference.insert(*iter);
                iter = context.pruneDomain(v, iter, 0);
            } else {
                iter++;
            }
        }
        ret[0] = firstDifference;
        for (int i=1;i<context.getPrefixK();i++) {
            std::set<int> difference;
            std::set_difference(context.getDomain(v, i).begin(), context.getDomain(v, i).end(),
                                context.getDomain(v, 0).begin(), context.getDomain(v, 0).end(),
                                std::inserter(difference,difference.begin()));
            ret[i] = difference;
            context.setDomain(v,context.getDomain(v,0), i);
        }
    } else {
        std::set<int> firstDifference;
        std::set<Variable_r> othersTmp = mFirstExpr.getVariables();
        othersTmp.erase(v);
        std::vector<Variable_r> others(othersTmp.begin(), othersTmp.end());
        others.push_back(mVariable);

        for (auto iter = context.getDomain(v, 0).begin(); iter != context.getDomain(v, 0).end(); ) {
            context.setAssignment(v, *iter, 0);
            if (shouldPrune(context, others, 0)) {
                firstDifference.insert(*iter);
                iter = context.pruneDomain(v, iter, 0);
            } else {
                iter++;
            }
        }
        ret[0] = firstDifference;
        // we don't constrain any future values of the expression inside the "first" clause
    }
    return ret;
}

bool PrimitiveFirstConstraint::shouldPrune(SearchNode& context,
                                              std::vector<Variable_r>& vars,
                                              int index) {
    if (index == (int)vars.size()) {
        return !isSatisfied(context, 0);
    }
    bool prune = true;
    Variable_r& v = vars[index];
    for (int i : context.getDomain(v, 0)) {
        context.setAssignment(v, 0, i);
        prune &= shouldPrune(context, vars, index + 1);
        if (!prune) {
            return false;
        }
    }
    return true;
}

bool PrimitiveFirstConstraint::lt(const Constraint &rhs) const {
    if (typeid(*this).before(typeid(rhs))) {
        return true;
    } else if (typeid(*this) == typeid(rhs)) {
        const PrimitiveFirstConstraint &p = static_cast<const PrimitiveFirstConstraint&>(rhs);
        if (mVariableExpr < p.mVariableExpr) {
            return true;
        } else if (mVariableExpr == p.mVariableExpr) {
            return mFirstExpr < p.mFirstExpr;
        }
    }
    return false;
}
bool PrimitiveFirstConstraint::eq(const Constraint &rhs) const {
    if (typeid(*this) == typeid(rhs)) {
        const PrimitiveFirstConstraint &p = static_cast<const PrimitiveFirstConstraint&>(rhs);
        return (mVariableExpr == p.mVariableExpr) && (mFirstExpr == p.mFirstExpr);
    }
    return false;
}
