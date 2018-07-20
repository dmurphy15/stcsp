#pragma once

class InstantaneousCSP
{
public:
    StateNode(std::set<Constraint *> constraints, std::set<Variable *> variables);

    StateNode *getNextState; this will be the iterator

    addChildNode(StateNode *node); this lets the solver set individual child nodes. so the solver will get a statenode from the iterator,
                    then look for previous equivalent statenodes. If it finds a previous one, it will add that as the
                    child and get the next state node etc, if it does not, then it adds the new one as the child and
                    recurs on it, before getting the next state node

    all constraints (and expressions) need to have a hash function so we can determine equivalence. Perhaps a function that
    combines the recursive hashes of its direct components plus its contraint(/expression) type
private:
    std::set<Constraint *> mConstraints;
    std::set<Variable *> mVariables;
    VariableChooser mVariableChooser;
    std::vector<StateNode *> mChildNodes;
};

REPLACE ALL UNORDERED_SETS with regular, ordered, sets, or at least think about it.
One issue with sets of pointers is that it would compare identity rather than making sure the constriaints
have the same values (are the same type and have the same member expressions). But if we are careful to
never copy constraints after we begin solving, we may be alright

future optimization - instead of iterating through all bottommost variables contained by the constraints
directly, break them up so the expressions can create equivalent variables whose domains are sort of
arrays whose initial elements represent combinations of values of the input variables and whose last
element represents the expression's evaluated value - memory inefficient
could use this for binarization to use AC-3 algorithm
