#pragma once

#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "BCRangeDomain.h"
#include "GACRangeDomain.h"

/***********************************************************************************************************************
 * some useful aliases and forward declarations
 *
 * IF YOU WANT TO USE A DIFFERENT DATA STRUCTURE FOR A VARIABLE'S DOMAIN (like BCRangeDomain, GACRangeDomain, set<int>),
 * SEE BELOW
 * ********************************************************************************************************************/

class Variable;
using Variable_r = std::reference_wrapper<Variable>;
bool operator==(const Variable &a, const Variable &b);
bool operator<(const Variable &a, const Variable &b);
class Expression;
using Expression_r = std::reference_wrapper<Expression>;
bool operator==(const Expression &a, const Expression &b);
bool operator<(const Expression &a, const Expression &b);
class Constraint;
using Constraint_r = std::reference_wrapper<Constraint>;
bool operator==(const Constraint &a, const Constraint &b);
bool operator<(const Constraint &a, const Constraint &b);
class SearchNode;
using SearchNode_r = std::reference_wrapper<SearchNode>;

using assignment_t = std::map<Variable_r, int>;
// a valid domain class must have similar functionality to a set of ints;
// it should have similar methods and iterate through its values in sorted order
using domain_t = BCRangeDomain;// GACRangeDomain;// std::set<int>;

using namespace std::__cxx11;
#include "boost/coroutine2/all.hpp"
#include "boost/bind.hpp"
using coro_assignment_t = boost::coroutines2::coroutine<assignment_t>;
using coro_int_t = boost::coroutines2::coroutine<int>;