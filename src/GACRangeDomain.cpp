#include "../include/GACRangeDomain.h"

#include <algorithm>
#include <vector>

/**
 * Representation of a domain. Hopefully replacement for set of ints.
 * A sorted set of pairs of integers. The pairs represent disjoint ranges of integers that make up the domain;
 * the pair (a, b) represents the range [a, b).
 */

GACRangeDomain::iterator GACRangeDomain::begin() const {
    if (mSize == 0) {
        return end();
    }
    return GACRangeDomain::iterator(&mRanges, (*mRanges.begin()).first, mRanges.begin());
}

GACRangeDomain::iterator GACRangeDomain::end() const {
    if (mSize == 0) {
        return GACRangeDomain::iterator(&mRanges, -7, mRanges.end());
    }
    return GACRangeDomain::iterator(&mRanges, (*--mRanges.end()).second, --mRanges.end());
}

std::reverse_iterator<GACRangeDomain::iterator> GACRangeDomain::rbegin() const {
    return std::reverse_iterator<GACRangeDomain::iterator>(end());
}

std::reverse_iterator<GACRangeDomain::iterator> GACRangeDomain::rend() const {
    return std::reverse_iterator<GACRangeDomain::iterator>(begin());
}

GACRangeDomain::iterator GACRangeDomain::insert(int val) {
    insert({val, val+1});
    return find(val);
}

void GACRangeDomain::insert(std::pair<int, int> range) {
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

void GACRangeDomain::insert(GACRangeDomain& d) {
    for (auto r : d.mRanges) {
        insert(r);
    }
}

template <typename T>
void GACRangeDomain::insert(T start, T finish) {
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
template void GACRangeDomain::insert(GACRangeDomain::iterator start, GACRangeDomain::iterator finish);
template void GACRangeDomain::insert(std::set<int>::iterator start, std::set<int>::iterator finish);
template void GACRangeDomain::insert(std::vector<int>::iterator start, std::vector<int>::iterator finish);

GACRangeDomain::iterator GACRangeDomain::erase(GACRangeDomain::iterator it) {
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

GACRangeDomain::iterator GACRangeDomain::find(int val) const {
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

GACRangeDomain::iterator GACRangeDomain::at(int where) const {
    int idx = 0;
    auto it = mRanges.begin();
    for (; idx + (*it).second - (*it).first < where; ++it) {
        idx += (*it).second - (*it).first;
    }
    return iterator(&mRanges, (*it).first + where - idx, it);
}

GACRangeDomain GACRangeDomain::slice(int from, int to) {
    GACRangeDomain d = GACRangeDomain();
    iterator start = at(from);
    iterator finish = at(to);
    d.insert(start, finish);
    return d;
}

// this really should not be inlined for some reason - whenever it's inlined it breaks things
int& __attribute__ ((noinline)) GACRangeDomain::iterator::operator*() {
    asm("");
    return mVal;
}