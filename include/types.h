#pragma once

#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "Domain2.h"

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
// performance appears to be slightly better with unordered_set
using domain_t = Domain2;//std::set<int>;

using namespace std::__cxx11;
#include "boost/coroutine2/all.hpp"
#include "boost/bind.hpp"
using coro_assignment_t = boost::coroutines2::coroutine<assignment_t>;
using coro_int_t = boost::coroutines2::coroutine<int>;