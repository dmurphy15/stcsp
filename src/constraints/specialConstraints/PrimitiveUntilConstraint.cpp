#include "../../../include/constraints/specialConstraints/PrimitiveUntilConstraint.h"

#include "../../../include/expressions/specialExpressions/VariableExpression.h"
#include "../../../include/Variable.h"
#include "../../../include/SearchNode.h"

PrimitiveUntilConstraint::PrimitiveUntilConstraint(VariableExpression &variable, VariableExpression &untilVariable, int expressionSetId) :
        Constraint({variable, untilVariable}, false, expressionSetId),
        mVariable(variable.mVariable),
        mUntilVariable(untilVariable.mVariable) {}

PrimitiveUntilConstraint::~PrimitiveUntilConstraint() {}

void PrimitiveUntilConstraint::normalize(std::set<Constraint_r> &constraintList,
                                        std::map<Expression_r, Expression_r> &normalizedMap,
                                        std::set<Variable_r> &variableList)
{
    throw std::logic_error("this should have only been produced through normalization, so the contents should have already been normalized");
}

bool PrimitiveUntilConstraint::isSatisfied(SearchNode &context, int time) const
{
    for (int i=0; i < time; i++) {
        if (mUntilVariable.evaluate(context, i) != 0) {
            return true;
        }
    }
    return (mVariable.evaluate(context, time) != 0) || (mUntilVariable.evaluate(context, time) != 0);
}

// could have used defaultPropagate here, but I think this is faster
std::map<Variable_r, std::vector<std::set<int>>> PrimitiveUntilConstraint::propagate(SearchNode& context)
{
    std::map<Variable_r, std::vector<std::set<int>>> retMap;
    for (Variable_r v : getVariables()) {
        std::vector<std::set<int>>& ret = retMap[v];
        ret.resize(context.getPrefixK());

        Variable &other = v == mVariable ? mUntilVariable : mVariable;
        for (int i = 0; i < context.getPrefixK(); i++) {
            // if other has to be 0, and mUntilVariable cannot have previously been not-0, then we cannot be 0
            if (context.getDomain(other, i).size() == 1 && *context.getDomain(other, i).begin() == 0) {
                auto it = context.getDomain(v, i).find(0);
                if (it != context.getDomain(v, i).end()) {
                    context.pruneDomain(v, it, i);
                    ret[i] = {0};
                }
            }
            // mUntilVariable can be not-0 as of now, so all future timepoints need not be constrained
            bool done = false;
            for (int j : context.getDomain(mUntilVariable, i)) {
                if (j != 0) {
                    done = true;
                    break;
                }
            }
            if (done) {
                break;
            }
        }
    }
    return retMap;
}

