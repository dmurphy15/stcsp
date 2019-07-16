#include "../include/Domain.h"

#include <algorithm>
#include <vector>
#include <iostream>

/**
 * Representation of a domain. Hopefully replacement for set of ints.
 * A sorted set of pairs of integers. The pairs represent disjoint ranges of integers that make up the domain;
 * the pair (a, b) represents the range [a, b).
 */
Domain::Domain(const std::set<int> &vals) {
    if (vals.size() == 0) {
        mRange = {0,0};
        return;
    }
    mRange = {*std::min_element(vals.begin(), vals.end()), *std::max_element(vals.begin(), vals.end())+1};
}

Domain::Domain(std::initializer_list<int> vals) {
    std::vector<int> v(vals);
    if (vals.size() == 0) {
        mRange = {0,0};
        return;
    }
    mRange = {*std::min_element(v.begin(), v.end()), *std::max_element(v.begin(), v.end())+1};
}

Domain::iterator Domain::begin() const {
    return Domain::iterator(mRange.first);
}

Domain::iterator Domain::end() const {
    return Domain::iterator(mRange.second);
}

std::reverse_iterator<Domain::iterator> Domain::rbegin() const {
    return std::reverse_iterator<Domain::iterator>(end());
}

std::reverse_iterator<Domain::iterator> Domain::rend() const {
    return std::reverse_iterator<Domain::iterator>(begin());
}

Domain::iterator Domain::insert(int val) {
    if (size() == 0) {
        mRange = {val, val+1};
        return iterator(val);
    } else {
        mRange = {std::min(val, mRange.first), std::max(val + 1, mRange.second)};
        return iterator(val);
    }
}

void Domain::insert(iterator start, iterator finish)
{
    if (start == finish) {
        return;
    } else if (size() == 0) {
        mRange = {*start, *(--finish)+1};
    } else {
        mRange = {std::min(*start, mRange.first), std::max(*(--finish) + 1, mRange.second)};
    }
}

void Domain::insert(std::set<int>::iterator start, std::set<int>::iterator finish)
{
    if (start == finish) {
        return;
    } else if (size() == 0) {
            mRange = {*start, *(--finish)+1};
    } else {
        mRange = {std::min(*start, mRange.first), std::max(*(--finish) + 1, mRange.second)};
    }
}

Domain::iterator Domain::erase(Domain::iterator &it) {
    if (*it == mRange.first) {
        mRange.first++;
        return iterator(mRange.first);
    }
    if (*it == mRange.second-1) {
        mRange.second--;
        return iterator(mRange.second);
    }
}

Domain::iterator Domain::find(int val) const {
    if ((val < mRange.second) && (val >= mRange.first)) {
        return Domain::iterator(val);
    } else {
        return Domain::iterator(mRange.second);
    }
}

Domain::iterator Domain::at(int where) const {
    return Domain::iterator(mRange.first+where);
}

Domain Domain::slice(int from, int to) {
    if (from == to) {
        return Domain();
    }
    return Domain({mRange.first+from, mRange.first+to-1});
}

// not inlining, bc that caused problems when compiler tried to optimize
int& Domain::iterator::operator*() {
    mVals[mIndex] = mVal;
    return mVals[mIndex++];
}