#include "../../include/expressions/FirstExpression.h"

#include "../../include/SearchNode.h"
#include "../../include/expressions/specialExpressions/ConstantExpression.h"
#include "../../include/constraints/specialConstraints/EqualConstraint.h"

FirstExpression::FirstExpression(Expression &a) : Expression({a}, false), mExpr(a) {}

int FirstExpression::evaluate(SearchNode &context, int time) const
{
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!WARNING: THIS ONLY WORKS IF WE ASSUME THAT WE ARE PROGRESSING DEPTH-FIRST THROUGH THE SEARCHNODE TREE
    // this allows us to say that the root searchnode will still have the proper assignments to variables that we want,
    // which are the ones that would lead us to the searchnode we're currently at
    // we ensured this by adding a part in Solver that checks if the current node is root, and if so it sets the first assignment
    // to correspond to the most recent solution, so we can now call evaluate without things being screwed up (since we are fully exploring each branch from root)
    return mExpr.evaluate(*SearchNode::root, 0);
}

Expression& FirstExpression::normalize(std::set<Constraint_r> &constraintList, std::set<Variable_r> &variableList) {
    // adding this constraint does absolutely nothing, except since it is a tautology, the solver will remove it
    // after the first timepoint
    // this is used to differentiate the time when the FirstExpression is constant (after the root node has been solved)
    // from the time when the FirstExpression can take variable values (while solving the root node)
    // VERY HACKY, but then again if we don't want to do special normalization for FirstExpressions, propagating them
    // will be weird
//    ConstantExpression& c = *new ConstantExpression(0);
//    constraintList.insert(*new EqualConstraint(c, c));

    return *new FirstExpression(mExpr.normalize(constraintList, variableList));
}

domain_t FirstExpression::getDomain(SearchNode &context, int time) const
{
    if (context.id == SearchNode::ROOT_ID) {
        return mExpr.getDomain(context, 0);
    } else {
        return {mExpr.evaluate(*SearchNode::root, 0)};
    }
}

domain_t FirstExpression::getInitialDomain() const
{
    return mExpr.getInitialDomain();
}

std::set<Variable_r> FirstExpression::getVariables(bool root) const
{
    if (!root) { // after the root node, this expression is effectively constant
        return {};
    }
    return mExpr.getVariables(root);
}

Expression& FirstExpression::freezeFirstExpressions() {
    return *new ConstantExpression(evaluate(*SearchNode::root, 0));
}
