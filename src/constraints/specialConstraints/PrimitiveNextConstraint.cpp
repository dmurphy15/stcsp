#include "../../../include/constraints/specialConstraints/PrimitiveNextConstraint.h"

#include <algorithm>
#include <stdexcept>

#include "../../../include/SearchNode.h"
#include "../../../include/expressions/specialExpressions/VariableExpression.h"
#include "../../../include/Variable.h"
#include "../../../include/Expression.h"

PrimitiveNextConstraint::PrimitiveNextConstraint(VariableExpression &varExpr, VariableExpression &nextVarExpr) :
        mVarExpr(varExpr),
        mNextVarExpr(nextVarExpr),
        mVariable(*varExpr.getVariables().begin()),
        mNextVariable(*nextVarExpr.getVariables().begin()){}

PrimitiveNextConstraint::~PrimitiveNextConstraint() {}

void PrimitiveNextConstraint::normalize(std::set<Constraint_r> &constraintList,
                                std::set<Variable_r> &variableList)
{
    // can do this since normalizing the member variableExpressions would do nothing anyway
    constraintList.insert(*this);
}

int PrimitiveNextConstraint::isSatisfied(SearchNode &context, int time) const
{
    if (context.getPrefixK() < 2) {
        throw std::logic_error("can't evaluate with prefix k less than 2 - reconsider this, maybe just return true");
    } else {
        return (time == context.getPrefixK()) || mVarExpr.evaluate(context, time) == mNextVarExpr.evaluate(context, time + 1);
    }
}

std::set<Variable_r> PrimitiveNextConstraint::getVariables() const
{
    std::set<Variable_r> vars1 = mNextVarExpr.getVariables();
    vars1.insert(*mVarExpr.getVariables().begin());
    return vars1;
}

// this will work even if v is both mVariable and mNextVariable
std::vector<std::set<int>> PrimitiveNextConstraint::propagate(Variable &v, SearchNode &context) {
    //TODO was able to use set_intersection here bc I new I was iterating over variableExpressions, so there would be no
    //TODO exponential explosion for iterating through their domains. (On the other hand, I already had to sacrifice that
    //TODO exponential explosion when creating this constraint to reduce it to 2 variableExpressions)

    std::vector<std::set<int>> differences(context.getPrefixK());
    if (context.getPrefixK() < 2) {
        return differences;
    }
    if (v == mVariable){
        propagateHelper(v, context, false, differences);
    }
    if (v == mNextVariable){
        propagateHelper(v, context, true, differences);
    }
    return differences;
}

void PrimitiveNextConstraint::propagateHelper(Variable &v,
                                              SearchNode &context,
                                              bool vIsNext,
                                              std::vector<std::set<int>>& ret)
{
    for (int i=0; i < context.getPrefixK() - 1; i++) {
        std::set<int> currDomain(context.getDomain(mVariable, i).begin(), context.getDomain(mVariable, i).end());
        std::set<int> nextDomain(context.getDomain(mNextVariable, i+1).begin(), context.getDomain(mNextVariable, i+1).end());
        std::set<int> intersection;
        std::set<int> difference;
        std::set_intersection(nextDomain.begin(), nextDomain.end(),
                              currDomain.begin(), currDomain.end(),
                              std::inserter(intersection,intersection.begin()));
        std::set_difference(nextDomain.begin(), nextDomain.end(),
                            currDomain.begin(), currDomain.end(),
                            std::inserter(difference,difference.begin()));
        domain_t newDomain(intersection.begin(), intersection.end());
        context.setDomain(v, newDomain, i + vIsNext);
        ret[i+vIsNext].insert(difference.begin(), difference.end());
    }
}


bool PrimitiveNextConstraint::lt(const Constraint &rhs) const {
    if (typeid(*this).before(typeid(rhs))) {
        return true;
    } else if (typeid(*this) == typeid(rhs)) {
        const PrimitiveNextConstraint &p = static_cast<const PrimitiveNextConstraint&>(rhs);
        if (mVarExpr < p.mVarExpr) {
            return true;
        } else if (mVarExpr == p.mVarExpr) {
            return mNextVarExpr < p.mNextVarExpr;
        }
    }
    return false;
}
bool PrimitiveNextConstraint::eq(const Constraint &rhs) const {
    if (typeid(*this) == typeid(rhs)) {
        const PrimitiveNextConstraint &p = static_cast<const PrimitiveNextConstraint&>(rhs);
        return (mVarExpr == p.mVarExpr) && (mNextVarExpr == p.mNextVarExpr);
    }
    return false;
}
