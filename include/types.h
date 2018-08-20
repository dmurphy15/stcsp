#pragma once

#include <map>
#include <set>
#include <unordered_set>
#include <vector>
#include <functional>
#include "boost/coroutine2/all.hpp"
#include "boost/bind.hpp"

class Variable;
using Variable_r = std::reference_wrapper<Variable>;
class Expression;
using Expression_r = std::reference_wrapper<Expression>;
class Constraint;
using Constraint_r = std::reference_wrapper<Constraint>;
class SearchNode;
using SearchNode_r = std::reference_wrapper<SearchNode>;

using coro_assignment_t = boost::coroutines2::coroutine<std::map<std::reference_wrapper<Variable>, int>>;
using coro_int_t = boost::coroutines2::coroutine<int>;

using assignment_t = std::map<Variable_r, int>;
// performance appears to be slightly better with unordered_set
using domain_t = std::unordered_set<int>;