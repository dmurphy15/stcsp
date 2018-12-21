#include "../include/Domain.h"

/**
 * Representation of a domain. Hopefully replacement for set of ints.
 * A sorted set of pairs of integers. The pairs represent disjoint ranges of integers that make up the domain;
 * the pair (a, b) represents the range [a, b).
 */
Domain::Domain(std::set<int> &&vals) {
    for (auto it=vals.begin(); it != vals.end(); it++) {
        insert(*it);
    }
}

Domain::iterator Domain::begin() {
    if (mRanges.empty()) { return Domain::iterator(mRanges.begin(), mRanges.begin(), mRanges.begin(), -1); }
    return Domain::iterator(mRanges.begin(), mRanges.end(), mRanges.begin(), (*mRanges.begin()).first);
}

Domain::iterator Domain::end() {
    return Domain::iterator(mRanges.begin(), mRanges.end(), mRanges.end(), -1);
}

Domain::iterator Domain::insert(int val) {
    std::pair<int, int> toInsert(val, val+1);
    std::set<std::pair<int, int>>::iterator after = mRanges.lower_bound(toInsert);
    bool mergeBefore = false, mergeAfter = false;
    if (after != mRanges.end() && val == (*after).first - 1) { // prepend to the start of a range
        toInsert.second = (*after).second;
        mergeAfter = true;
    }
    if (after != mRanges.begin() && val == (*std::prev(after)).second) { // append to the end of a range
        toInsert.first = (*std::prev(after)).first;
        mergeBefore = true;
    }
    if (mergeBefore) {
        after = mRanges.erase(std::prev(after));
    }
    if (mergeAfter) {
        after = mRanges.erase(after);
    }
    std::set<std::pair<int, int>>::iterator it = mRanges.insert(after, toInsert); // here after is being used as a hint to get constant-time insertion
    return Domain::iterator(mRanges.begin(), mRanges.end(), it, val);
}

Domain::iterator Domain::erase(Domain::iterator &it) {
    int begin = (*(it.mRangeIterator)).first;
    int end = (*(it.mRangeIterator)).second;
    std::set<std::pair<int, int>>::iterator next = mRanges.erase(it.mRangeIterator);
    if (it.mVal == end - 1) {
        // check if we removed an entire range; don't want to reinsert ranges of size 0
        if (begin != end-1) {
            next = mRanges.insert(next, {begin, end-1});
            next++;
        }
        // check if there are no elements after us
        if (next == mRanges.end()) {
            return Domain::iterator(mRanges.begin(), mRanges.end(), next, -1);
        }
    } else if (it.mVal == begin) {
        next = mRanges.insert(next, {begin+1, end});
    } else {
        next = mRanges.insert(next, {begin, it.mVal});
        next = mRanges.insert(next, {it.mVal+1, end});
    }
    return Domain::iterator(mRanges.begin(), mRanges.end(), next, (*next).first);
}