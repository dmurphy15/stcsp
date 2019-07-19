#include "../include/Domain3.h"

#include <algorithm>
#include <vector>

/**
 * Representation of a domain. Hopefully replacement for set of ints.
 * A sorted set of pairs of integers. The pairs represent disjoint ranges of integers that make up the domain;
 * the pair (a, b) represents the range [a, b).
 */

Domain3::iterator Domain3::begin() const {
    if (mSize == 0) {
        return end();
    }
    return Domain3::iterator(&mRanges, (*mRanges.begin()).first, mRanges.begin());
}

Domain3::iterator Domain3::end() const {
    if (mSize == 0) {
        return Domain3::iterator(&mRanges, -7, mRanges.end());
    }
    return Domain3::iterator(&mRanges, (*--mRanges.end()).second, --mRanges.end());
}

std::reverse_iterator<Domain3::iterator> Domain3::rbegin() const {
    return std::reverse_iterator<Domain3::iterator>(end());
}

std::reverse_iterator<Domain3::iterator> Domain3::rend() const {
    return std::reverse_iterator<Domain3::iterator>(begin());
}

Domain3::iterator Domain3::insert(int val) {
    insert({val, val+1});
    return find(val);
}

void Domain3::insert(std::pair<int, int> range) {
    if (range.first >= range.second) {
        return;
    }
    int addedSize = range.second - range.first;
    bool mergeLeft = false, mergeRight = false;
    auto it = mRanges.lower_bound(range); // it will point to a range whose start is >= our start
    if (it != mRanges.end() && range.second >= (*it).first) {
        addedSize -= range.second - (*it).first;
        range.second = std::max(range.second, (*it).second);
        mergeRight = true;
    }
    auto it2 = it;
    if (it2 != mRanges.begin() && range.first <= (*--it2).second) { // --it will point to a range whose start is <= our start
        addedSize -= (*it2).second - range.first;
        range.first = (*it2).first;
        range.second = std::max(range.second, (*it2).second);
        mergeLeft = true;
    }
    if (mergeLeft) {
        mRanges.erase(it2);
    }
    if (mergeRight) {
        it = mRanges.erase(it);
    }
    mRanges.insert(it, range);
    mSize += addedSize;
}

void Domain3::insert(Domain3& d) {
    for (auto r : d.mRanges) {
        insert(r);
    }
}

template <typename T>
void Domain3::insert(T start, T finish) {
    if (start == finish) {
        return;
    }
    auto s = start;
    int prev = *s;
    std::pair<int, int> currentRange = {prev, prev+1};
    ++s;
    for (; s != finish; ++s) {
        if (*s == prev + 1) {
            currentRange.second++;
        } else {
            insert(currentRange);
            currentRange = {*s, *s+1};
        }
    }
    insert(currentRange);
}
template void Domain3::insert(Domain3::iterator start, Domain3::iterator finish);
template void Domain3::insert(std::set<int>::iterator start, std::set<int>::iterator finish);
template void Domain3::insert(std::vector<int>::iterator start, std::vector<int>::iterator finish);

Domain3::iterator Domain3::erase(Domain3::iterator it) {
    std::pair<int, int> range1 = *(it.mRangeIt);
    std::pair<int, int> range2 = range1;
    auto hint = mRanges.erase(it.mRangeIt);

    range1.second = *it;
    range2.first = range1.second + 1;

    --mSize;
    if (range1.first < range1.second) {
        mRanges.insert(hint, range1);
    }
    if (range2.first < range2.second) {
        mRanges.insert(hint, range2);

        --hint;
        return iterator(&mRanges, hint->first, hint);
    } else {
        return end();
    }
}

Domain3::iterator Domain3::find(int val) const {
    auto it = mRanges.lower_bound({val, val+1});
    if (it != mRanges.end() && (*it).first < val+1) {
        return iterator(&mRanges, val, it);
    }
    auto it2 = it;
    if (it2 != mRanges.begin() && (*--it2).second > val) {
        return iterator(&mRanges, val, it2);
    }
    return end();
}

Domain3::iterator Domain3::at(int where) const {
    int idx = 0;
    auto it = mRanges.begin();
    for (; idx + (*it).second - (*it).first < where; ++it) {
        idx += (*it).second - (*it).first;
    }
    return iterator(&mRanges, (*it).first + where - idx, it);
}

Domain3 Domain3::slice(int from, int to) {
    Domain3 d = Domain3();
    iterator start = at(from);
    iterator finish = at(to);
    d.insert(start, finish);
    return d;
}

// this really should not be inlined for some reason - whenever it's inlined it breaks things
int& __attribute__ ((noinline)) Domain3::iterator::operator*() {
    asm("");
    return mVal;
}