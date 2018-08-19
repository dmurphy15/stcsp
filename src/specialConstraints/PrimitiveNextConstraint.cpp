#include "../../include/specialConstraints/PrimitiveNextConstraint.h"

#include <algorithm>
#include <stdexcept>

#include "../../include/SearchNode.h"
#include "../../include/specialExpressions/VariableExpression.h"
#include "../../include/Variable.h"
#include "../../include/Expression.h"

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

// defaultPropagate is only intended for looking at 1 timepoint at a time, so it would not have worked
//// NOTE: DUE TO OUR NORMALIZATION TECHNIQUE, V WILL ONLY EVER APPEAR IN ONE OF MVARIABLEEXPRESSION OR MNEXTEXPRESSION

//// nevermind, this will work even if it appears in both
std::vector<std::set<int>> PrimitiveNextConstraint::propagate(Variable &v, SearchNode &context) {
    //TODO was able to use set_intersection here bc I new I was iterating over variableExpressions, so there would be no
    //TODO exponential explosion for iterating through their domains. (On the other hand, I already had to sacrifice that
    //TODO exponential explosion when creating this constraint to reduce it to 2 variableExpressions)

    std::vector<std::set<int>> differences(context.getPrefixK());
    if (context.getPrefixK() < 2) {
        return differences;
    }
    if (v == mVariable){
        for (int i=0; i<context.getPrefixK() - 1; i++) {
            domain_t currDomain = mVarExpr.getDomain(context, i);
            domain_t nextDomain = mNextVarExpr.getDomain(context, i+1);
            domain_t intersection;
            std::set<int> difference;
            std::set_intersection(currDomain.begin(), currDomain.end(),
                                  nextDomain.begin(), nextDomain.end(),
                                  std::inserter(intersection,intersection.begin()));
            std::set_difference(currDomain.begin(), currDomain.end(),
                                nextDomain.begin(), nextDomain.end(),
                                std::inserter(difference,difference.begin()));

            context.setDomain(v, intersection, i);
            differences[i] = difference;
        }
    }
    if (v == mNextVariable){
        for (int i=1; i<context.getPrefixK(); i++) {
            domain_t currDomain = mVarExpr.getDomain(context, i-1);
            domain_t nextDomain = mNextVarExpr.getDomain(context, i);
            domain_t intersection;
            std::set<int> difference;
            std::set_intersection(nextDomain.begin(), nextDomain.end(),
                                  currDomain.begin(), currDomain.end(),
                                  std::inserter(intersection,intersection.begin()));
            std::set_difference(nextDomain.begin(), nextDomain.end(),
                                currDomain.begin(), currDomain.end(),
                                std::inserter(difference,difference.begin()));

            context.setDomain(v, intersection, i);
            differences[i].insert(difference.begin(), difference.end());
        }
    }
    return differences;
}
//    } else if (v == *mVariableExpr.getVariables().begin()){
//        std::vector<std::set<int>> differences;
//        for (int i=0; i<context.getPrefixK() - 1; i++) {
//            domain_t nextDomain = mNextExpr.getDomain(context, i);
//            domain_t currDomain = mVariableExpr.getDomain(context, i);
//            domain_t intersection;
//            std::set<int> difference;
//            std::set_intersection(currDomain.begin(), currDomain.end(), nextDomain.begin(), nextDomain.end(), intersection.begin());
//            std::set_difference(currDomain.begin(), currDomain.end(), nextDomain.begin(), nextDomain.end(), difference.begin());
//            context.setDomain(v, intersection, i);
//            differences.push_back(difference);
//        }
//        return differences;
//    } else {
//        std::set<Variable_r> currVarsTmp = mVariableExpr.getVariables();
//        std::vector<Variable_r> currVars(currVarsTmp.begin(), currVarsTmp.end());
//        std::set<Variable_r> futureVarsTmp = mNextExpr.getVariables();
//        std::vector<Variable_r>::iterator position = std::find(futureVarsTmp.begin(), futureVarsTmp.end(), v); others.erase(position);
//        std::vector<Variable_r> futureVars(futureVarsTmp.begin(), futureVarsTmp.end());
//        std::vector<std::set<int>> differences;
//        for (int i=0; i<context.getPrefixK() - 1; i++) {
//            differences.push_back(propagateHelper(v, context, currVars, futureVars, 0, time));
//        }
//    }
//}

//std::set<int> PrimitiveNextConstraint::propagateHelper(Variable& v,
//                                                       SearchNode& context,
//                                                       std::vector<Variable_r>& currVars,
//                                                       std::vector<Variable_r>& futureVars,
//                                                       int index,
//                                                       int time) {
//    std::set<int> removals;
//    for (auto &iter = context.getDomain(v, time+1).begin(); iter != context.getDomain(v, time+1).end(); ) {
//        context.setAssignment(v, time+1, *iter);
//        if (shouldPrune(context, currVars, futureVars, 0, time)) {
//            removals.insert(*iter);
//            iter = context.pruneDomain(v, iter, time+1);
//        } else {
//            iter++;
//        }
//    }
//    return removals;
//}
//
//bool PrimitiveNextConstraint::shouldPrune(SearchNode& context,
//                                              std::vector<Variable_r>& currVars,
//                                              std::vector<Variable_r>& futureVars,
//                                              int index,
//                                              int time) {
//    if (index == currVars.size() + futureVars.size()) {
//        return !isSatisfied(context, time);
//    }
//    bool prune = true;
//    if (index < currVars.size()) {
//        Variable_r& v = currVars[index];
//        for (int i : context.getDomain(v, time)) {
//            context.setAssignment(v, time, i);
//            prune &= shouldPrune(context, currVars, futureVars, index + 1, time);
//            if (!prune) {
//                return false;
//            }
//        }
//    } else {
//        Variable_r& v = futureVars[index - currVars.size()];
//        for (int i : context.getDomain(v, time+1)) {
//            context.setAssignment(v, time+1, i);
//            prune &= shouldPrune(context, currVars, futureVars, index + 1, time);
//            if (!prune) {
//                return true;
//            }
//        }
//    }
//}

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
