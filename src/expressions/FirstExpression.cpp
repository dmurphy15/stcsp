#include "../../include/expressions/FirstExpression.h"

#include "../../include/SearchNode.h"

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
