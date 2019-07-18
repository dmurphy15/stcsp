#include "../include/Domain2.h"

#include <algorithm>
#include <vector>
#include <iostream>

/**
 * Representation of a domain. Hopefully replacement for set of ints.
 * A sorted set of pairs of integers. The pairs represent disjoint ranges of integers that make up the domain;
 * the pair (a, b) represents the range [a, b).
 */
Domain2::Domain2(const std::set<int> &vals) {
    if (vals.size() == 0) {
        mRange = {0,0};
        return;
    }
    mRange = {*std::min_element(vals.begin(), vals.end()), *std::max_element(vals.begin(), vals.end())+1};
}

Domain2::Domain2(std::initializer_list<int> vals) {
    std::vector<int> v(vals);
    if (vals.size() == 0) {
        mRange = {0,0};
        return;
    }
    mRange = {*std::min_element(v.begin(), v.end()), *std::max_element(v.begin(), v.end())+1};
}

Domain2::iterator Domain2::begin() const {
    return Domain2::iterator(mRange.first);
}

Domain2::iterator Domain2::end() const {
    return Domain2::iterator(mRange.second);
}

std::reverse_iterator<Domain2::iterator> Domain2::rbegin() const {
    return std::reverse_iterator<Domain2::iterator>(end());
}

std::reverse_iterator<Domain2::iterator> Domain2::rend() const {
    return std::reverse_iterator<Domain2::iterator>(begin());
}

Domain2::iterator Domain2::insert(int val) {
    if (size() == 0) {
        mRange = {val, val+1};
        return iterator(val);
    } else {
        mRange = {std::min(val, mRange.first), std::max(val + 1, mRange.second)};
        return iterator(val);
    }
}

void Domain2::insert(iterator start, iterator finish)
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

void Domain2::insert(std::set<int>::iterator start, std::set<int>::iterator finish)
{
    if (start == finish) {
        return;
    } else if (size() == 0) {
            mRange = {*start, *(--finish)+1};
    } else {
        mRange = {std::min(*start, mRange.first), std::max(*(--finish) + 1, mRange.second)};
    }
}

Domain2::iterator Domain2::erase(Domain2::iterator &it) {
    if (*it == mRange.first) {
        mRange.first++;
        return iterator(mRange.first);
    }
    if (*it == mRange.second-1) {
        mRange.second--;
        return iterator(mRange.second);
    }
}

Domain2::iterator Domain2::find(int val) const {
    if ((val < mRange.second) && (val >= mRange.first)) {
        return Domain2::iterator(val);
    } else {
        return Domain2::iterator(mRange.second);
    }
}

Domain2::iterator Domain2::at(int where) const {
    return Domain2::iterator(mRange.first+where);
}

Domain2 Domain2::slice(int from, int to) {
    if (from == to) {
        return Domain2();
    }
    return Domain2({mRange.first+from, mRange.first+to-1});
}

// not inlining, bc that caused problems when compiler tried to optimize
int& Domain2::iterator::operator*() {
    return mVal;
}