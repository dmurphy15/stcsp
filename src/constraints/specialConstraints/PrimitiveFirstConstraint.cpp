#include "../../../include/constraints/specialConstraints/PrimitiveFirstConstraint.h"

#include <algorithm>

#include "../../../include/expressions/specialExpressions/VariableExpression.h"
#include "../../../include/Variable.h"
#include "../../../include/SearchNode.h"

PrimitiveFirstConstraint::PrimitiveFirstConstraint(VariableExpression &variableExpr, Expression &firstExpr, int expressionSetId) :
        Constraint({variableExpr, firstExpr}, false, expressionSetId),
        mVariableExpr(variableExpr),
        mFirstExpr(firstExpr),
        mVariable(variableExpr.mVariable){}

PrimitiveFirstConstraint::~PrimitiveFirstConstraint() {}

void PrimitiveFirstConstraint::normalize(std::set<Constraint_r> &constraintList,
                                         std::map<Expression_r, Expression_r> &normalizedMap,
                                         std::set<Variable_r> &variableList)
{
    throw std::logic_error("this should have only been produced through normalization, so the contents should have already been normalized");
}

bool PrimitiveFirstConstraint::isSatisfied(SearchNode &context, int time) const
{
    return mVariableExpr.evaluate(context, time) == mFirstExpr.evaluate(context, 0);
}

// using generic GAC/BC style propagation, which only prunes one variable's domain at a time
// it may be faster to treat this as a global constraint and use a kind of set difference to prune the
// domains of all variables at once (similar note in EqualConstraint). Then again, it may not be faster...
std::map<Variable_r, std::vector<std::set<int>>> PrimitiveFirstConstraint::propagate(SearchNode &context)
{
    std::map<Variable_r, std::vector<std::set<int>>> retMap;
    const std::set<Variable_r>& variables = getVariables();
    auto varIter = variables.begin();
    while (varIter != variables.end()) {
        Variable& v = *varIter;
        std::vector<std::set<int>>& ret = retMap[v];
        ret.resize(context.getPrefixK());
        std::vector<Variable_r> others(variables.begin(), varIter);
        others.insert(others.end(), ++varIter, variables.end());
        if (v == mVariable) {
            // mVariable should be constant, so we can just propagate for 1 timepoint and duplicate the domain
            std::set<int> firstDifference;
//            for (auto iter = context.getDomain(v, 0).begin(); iter != context.getDomain(v, 0).end(); ) {
//                context.setAssignment(v, 0, *iter);
//                if (shouldPrune(context, others, others.begin())) {
//                    firstDifference.insert(*iter);
//                    iter = context.pruneDomain(v, iter, 0);
//                } else {
//                    iter++;
//                }
//            }



            for (auto iter = context.getDomain(v, 0).begin(); iter != context.getDomain(v, 0).end(); ) {
                context.setAssignment(v, 0, *iter);
                if (shouldPrune(context, others, others.begin())) {
                    firstDifference.insert(*iter);
                    iter = context.pruneDomain(v, iter, 0);
                } else {
                    break;
                }
            }
            for (auto iter = context.getDomain(v, 0).rbegin(); iter != context.getDomain(v, 0).rend(); ) {
                context.setAssignment(v, 0, *iter);
                if (shouldPrune(context, others, others.begin())) {
                    firstDifference.insert(*iter);
                    auto iter2 = ++iter.base();
                    iter2 = context.pruneDomain(v, iter2, 0);
                    iter = std::make_reverse_iterator(iter2);
                } else {
                    break;
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
//            for (auto iter = context.getDomain(v, 0).begin(); iter != context.getDomain(v, 0).end(); ) {
//                context.setAssignment(v, 0, *iter);
//                if (shouldPrune(context, others, others.begin())) {
//                    firstDifference.insert(*iter);
//                    iter = context.pruneDomain(v, iter, 0);
//                } else {
//                    iter++;
//                }
//            }




            for (auto iter = context.getDomain(v, 0).begin(); iter != context.getDomain(v, 0).end(); ) {
                context.setAssignment(v, 0, *iter);
                if (shouldPrune(context, others, others.begin())) {
                    firstDifference.insert(*iter);
                    iter = context.pruneDomain(v, iter, 0);
                } else {
                    break;
                }
            }
            for (auto iter = context.getDomain(v, 0).rbegin(); iter != context.getDomain(v, 0).rend(); ) {
                context.setAssignment(v, 0, *iter);
                if (shouldPrune(context, others, others.begin())) {
                    firstDifference.insert(*iter);
                    auto iter2 = ++iter.base();
                    iter2 = context.pruneDomain(v, iter2, 0);
                    iter = std::make_reverse_iterator(iter2);
                } else {
                    break;
                }
            }




            ret[0] = firstDifference;
            // we don't constrain any future values of the expression inside the "first" clause
        }
    }
    return retMap;
}

bool PrimitiveFirstConstraint::shouldPrune(SearchNode& context,
                                           std::vector<Variable_r>& vars,
                                           std::vector<Variable_r>::iterator index) {
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

