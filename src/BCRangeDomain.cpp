#include "../include/BCRangeDomain.h"

#include <algorithm>
#include <vector>

/**
 * Representation of a domain. Hopefully replacement for set of ints.
 * A sorted set of pairs of integers. The pairs represent disjoint ranges of integers that make up the domain;
 * the pair (a, b) represents the range [a, b).
 */
BCRangeDomain::BCRangeDomain(const std::set<int> &vals) {
    if (vals.size() == 0) {
        mRange = {0,0};
        return;
    }
    mRange = {*std::min_element(vals.begin(), vals.end()), *std::max_element(vals.begin(), vals.end())+1};
}

BCRangeDomain::BCRangeDomain(std::initializer_list<int> vals) {
    std::vector<int> v(vals);
    if (vals.size() == 0) {
        mRange = {0,0};
        return;
    }
    mRange = {*std::min_element(v.begin(), v.end()), *std::max_element(v.begin(), v.end())+1};
}

BCRangeDomain::iterator BCRangeDomain::begin() const {
    return BCRangeDomain::iterator(mRange.first);
}

BCRangeDomain::iterator BCRangeDomain::end() const {
    return BCRangeDomain::iterator(mRange.second);
}

std::reverse_iterator<BCRangeDomain::iterator> BCRangeDomain::rbegin() const {
    return std::reverse_iterator<BCRangeDomain::iterator>(end());
}

std::reverse_iterator<BCRangeDomain::iterator> BCRangeDomain::rend() const {
    return std::reverse_iterator<BCRangeDomain::iterator>(begin());
}

BCRangeDomain::iterator BCRangeDomain::insert(int val) {
    if (size() == 0) {
        mRange = {val, val+1};
        return iterator(val);
    } else {
        mRange = {std::min(val, mRange.first), std::max(val + 1, mRange.second)};
        return iterator(val);
    }
}

void BCRangeDomain::insert(iterator start, iterator finish)
{
    if (start == finish) {
        return;
    } else if (size() == 0) {
        mRange = {*start, *(--finish)+1};
        ++finish;
    } else {
        mRange = {std::min(*start, mRange.first), std::max(*(--finish) + 1, mRange.second)};
        ++finish;
    }
}

void BCRangeDomain::insert(std::set<int>::iterator start, std::set<int>::iterator finish)
{
    if (start == finish) {
        return;
    } else if (size() == 0) {
            mRange = {*start, *(--finish)+1};
    } else {
        mRange = {std::min(*start, mRange.first), std::max(*(--finish) + 1, mRange.second)};
    }
}

BCRangeDomain::iterator BCRangeDomain::erase(BCRangeDomain::iterator &it) {
    if (*it == mRange.first) {
        mRange.first++;
        return iterator(mRange.first);
    }
    if (*it == mRange.second-1) {
        mRange.second--;
        return iterator(mRange.second);
    }
}

BCRangeDomain::iterator BCRangeDomain::find(int val) const {
    if ((val < mRange.second) && (val >= mRange.first)) {
        return BCRangeDomain::iterator(val);
    } else {
        return BCRangeDomain::iterator(mRange.second);
    }
}

BCRangeDomain::iterator BCRangeDomain::at(int where) const {
    return BCRangeDomain::iterator(mRange.first+where);
}

BCRangeDomain BCRangeDomain::slice(int from, int to) {
    if (from == to) {
        return BCRangeDomain();
    }
    return BCRangeDomain({mRange.first+from, mRange.first+to-1});
}

// not inlining, bc that caused problems when compiler tried to optimize
int&  __attribute__ ((noinline)) BCRangeDomain::iterator::operator*() {
    asm("");
    return mVal;
}