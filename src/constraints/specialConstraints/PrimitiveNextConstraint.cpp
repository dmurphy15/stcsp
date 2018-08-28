#include "../../../include/constraints/specialConstraints/PrimitiveNextConstraint.h"

#include <algorithm>
#include <stdexcept>

#include "../../../include/SearchNode.h"
#include "../../../include/expressions/specialExpressions/VariableExpression.h"
#include "../../../include/Variable.h"
#include "../../../include/Expression.h"

PrimitiveNextConstraint::PrimitiveNextConstraint(VariableExpression &varExpr, VariableExpression &nextVarExpr) :
        Constraint({varExpr, nextVarExpr}, false),
        mVarExpr(varExpr),
        mNextVarExpr(nextVarExpr),
        mVariable(varExpr.mVariable),
        mNextVariable(nextVarExpr.mVariable){}

PrimitiveNextConstraint::~PrimitiveNextConstraint() {}

void PrimitiveNextConstraint::normalize(std::set<Constraint_r> &constraintList,
                                std::set<Variable_r> &variableList)
{
    throw std::logic_error("this should have only been produced through normalization, so the contents should have already been normalized");
}

bool PrimitiveNextConstraint::isSatisfied(SearchNode &context, int time) const
{
    if (context.getPrefixK() < 2) {
        throw std::logic_error("can't evaluate with prefix k less than 2 - reconsider this, maybe just return true");
    } else {
        return (time == context.getPrefixK()) || mVarExpr.evaluate(context, time) == mNextVarExpr.evaluate(context, time + 1);
    }
}

// this will work even if v is both mVariable and mNextVariable
std::vector<std::set<int>> PrimitiveNextConstraint::propagate(Variable &v, SearchNode &context) {
    std::vector<std::set<int>> differences(context.getPrefixK());
    if (context.getPrefixK() < 2) {
        return differences;
    }
    if (v == mVariable && v == mNextVariable) {
        propagateHelper(true, context, differences);
        propagateHelper(false, context, differences);
    } else {
        propagateHelper(v == mNextVariable, context, differences);
    }
    return differences;
}

void PrimitiveNextConstraint::propagateHelper(bool vIsNext,
                                              SearchNode &context,
                                              std::vector<std::set<int>>& ret)
{
    Variable& v = vIsNext ? mNextVariable : mVariable;
    Variable& other = vIsNext ? mVariable : mNextVariable;
    for (int i=0; i < context.getPrefixK() - 1; i++) {
        const domain_t& vDom = context.getDomain(v, i + vIsNext);
        const domain_t& otherDom = context.getDomain(other, i + (!vIsNext));
        for (auto it = vDom.begin(); it != vDom.end(); ) {
            if (otherDom.find(*it) == otherDom.end()) {
                ret[i+vIsNext].insert(*it);
                it = context.pruneDomain(v, it, i+vIsNext);
            } else {
                it++;
            }
        }
    }
}

