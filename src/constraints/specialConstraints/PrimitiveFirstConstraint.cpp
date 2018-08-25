#include "../../../include/constraints/specialConstraints/PrimitiveFirstConstraint.h"

#include <algorithm>

#include "../../../include/expressions/specialExpressions/VariableExpression.h"
#include "../../../include/Variable.h"
#include "../../../include/SearchNode.h"

PrimitiveFirstConstraint::PrimitiveFirstConstraint(VariableExpression &variableExpr, Expression &firstExpr) :
        Constraint({variableExpr, firstExpr}, false),
        mVariableExpr(variableExpr),
        mFirstExpr(firstExpr),
        mVariable(variableExpr.mVariable){}

PrimitiveFirstConstraint::~PrimitiveFirstConstraint() {}

void PrimitiveFirstConstraint::normalize(std::set<Constraint_r> &constraintList,
                                         std::set<Variable_r> &variableList)
{
    throw std::logic_error("this should have only been produced through normalization, so the contents should have already been normalized");
}

bool PrimitiveFirstConstraint::isSatisfied(SearchNode &context, int time) const
{
    return mVariableExpr.evaluate(context, time) == mFirstExpr.evaluate(context, 0);
}

std::vector<std::set<int>> PrimitiveFirstConstraint::propagate(Variable &v, SearchNode &context)
{
    std::vector<std::set<int>> ret(context.getPrefixK());
    if (v == mVariable) {
        // mVariable should be constant, so we can just propagate for 1 timepoint and duplicate the domain
        std::set<int> firstDifference;
        std::set<Variable_r> others; mFirstExpr.getVariables(others);
        for (auto iter = context.getDomain(v, 0).begin(); iter != context.getDomain(v, 0).end(); ) {
            context.setAssignment(v, 0, *iter);
            if (shouldPrune(context, others, others.begin())) {
                firstDifference.insert(*iter);
                iter = context.pruneDomain(v, iter, 0);
            } else {
                iter++;
            }
        }
        ret[0] = firstDifference;
        for (int i=1;i<context.getPrefixK();i++) {
            std::set<int> difference;
            std::set<int> currDomain(context.getDomain(v, i).begin(), context.getDomain(v, i).end());
            std::set<int> firstDomain(context.getDomain(v, 0).begin(), context.getDomain(v, 0).end());
            std::set_difference(currDomain.begin(), currDomain.end(),
                                firstDomain.begin(), firstDomain.end(),
                                std::inserter(difference,difference.begin()));
            ret[i] = difference;
            context.setDomain(v,context.getDomain(v,0), i);
        }
    } else {
        std::set<int> firstDifference;
        std::set<Variable_r> others; mFirstExpr.getVariables(others);
        others.erase(v);
        others.insert(mVariable);

        for (auto iter = context.getDomain(v, 0).begin(); iter != context.getDomain(v, 0).end(); ) {
            context.setAssignment(v, 0, *iter);
            if (shouldPrune(context, others, others.begin())) {
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
                                           std::set<Variable_r>& vars,
                                           std::set<Variable_r>::iterator index) {
    if (index == vars.end()) {
        return !isSatisfied(context, 0);
    }
    bool prune = true;
    Variable& v = *index;
    for (int i : context.getDomain(v, 0)) {
        context.setAssignment(v, 0, i);
        prune &= shouldPrune(context, vars, std::next(index));
        if (!prune) {
            return false;
        }
    }
    return true;
}

