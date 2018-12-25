#include "../include/Domain.h"

/**
 * Representation of a domain. Hopefully replacement for set of ints.
 * A sorted set of pairs of integers. The pairs represent disjoint ranges of integers that make up the domain;
 * the pair (a, b) represents the range [a, b).
 */
Domain::Domain(const std::set<int> &vals) {
    for (auto&& val : vals) {
        insert(val);
    }
}

Domain::iterator Domain::begin() const {
    if (mRanges.empty()) { return end(); }
    return Domain::iterator(mRanges.begin(), mRanges.end(), mRanges.begin(), (*mRanges.begin()).first);
}

Domain::iterator Domain::end() const {
    return Domain::iterator(mRanges.begin(), mRanges.end(), mRanges.end(), -1);
}

std::reverse_iterator<Domain::iterator> Domain::rbegin() const {
    return std::reverse_iterator<Domain::iterator>(end());
}

std::reverse_iterator<Domain::iterator> Domain::rend() const {
    return std::reverse_iterator<Domain::iterator>(begin());
}

Domain::iterator Domain::insert(int val) {
    return insert({val, val+1});
}

Domain::iterator Domain::insert(std::pair<int, int> range) {
    if (mRanges.empty()) {
        mRanges.insert(range);
        return Domain::iterator(mRanges.begin(), mRanges.end(), mRanges.begin(), range.first);
    }
    int val = range.first;
    bool mergeBefore = false, mergeAfter = false;
    std::set<std::pair<int, int>>::iterator after = mRanges.upper_bound(range);
    if (after != mRanges.begin() && (*std::prev(after)).second >= range.first) {
        if ((*std::prev(after)).second >= range.second) {
            return Domain::iterator(mRanges.begin(), mRanges.end(), std::prev(after), val);
        }
        range.first = (*std::prev(after)).first;
        mergeBefore = true;
    }
    if (after != mRanges.end() && (*after).first <= range.second) {
        if ((*after).first == range.first) {
            return Domain::iterator(mRanges.begin(), mRanges.end(), after, val);
        }
        range.second = range.second > (*after).second ? range.second : (*after).second;
        mergeAfter = true;
    }
    if (mergeBefore) {
        after = mRanges.erase(std::prev(after));
    }
    if (mergeAfter) {
        after = mRanges.erase(after);
    }
    std::set<std::pair<int, int>>::iterator it = mRanges.insert(after, range); // here after is being used as a hint to get constant-time insertion
    return Domain::iterator(mRanges.begin(), mRanges.end(), it, val);
}

void Domain::insert(const Domain &d) {
    for (auto&& range : d.mRanges) {
        insert(range);
    }
}

Domain::iterator Domain::erase(Domain::iterator &it) {
    int beginVal = (*(it.mRangeIterator)).first;
    int endVal = (*(it.mRangeIterator)).second;
    std::set<std::pair<int, int>>::iterator next = mRanges.erase(it.mRangeIterator);
    if (it.mVal == endVal - 1) {
        // check if we removed an entire range; don't want to reinsert ranges of size 0
        if (beginVal != endVal-1) {
            next = mRanges.insert(next, {beginVal, endVal-1});
            next++;
        }
        // check if there are no elements after us
        if (next == mRanges.end()) {
            return end();
        }
    } else if (it.mVal == beginVal) {
        next = mRanges.insert(next, {beginVal+1, endVal});
    } else {
        next = mRanges.insert(next, {beginVal, it.mVal});
        next = mRanges.insert(next, {it.mVal+1, endVal});
    }
    return Domain::iterator(mRanges.begin(), mRanges.end(), next, (*next).first);
}

Domain::iterator Domain::find(int val) const {
    std::set<std::pair<int, int>>::iterator it = mRanges.upper_bound({val, val+1});
    if (it != mRanges.end() && (*it).first == val) {
        return Domain::iterator(mRanges.begin(), mRanges.end(), it, val);
    } else if (it != mRanges.begin() && (*std::prev(it)).second > val) {
        return Domain::iterator(mRanges.begin(), mRanges.end(), std::prev(it), val);
    } else {
        return end();
    }
}

Domain::iterator Domain::at(int where) const {
    int idx = -1;
    int next = 0;
    std::set<std::pair<int, int>>::iterator it;
    for (it = mRanges.begin(); it != mRanges.end(); it++) {
        next = idx + (*it).second - (*it).first;
        if (next < where) {
            idx = next;
        } else {
            break;
        }
    }
    if (next < where) { return end(); }
    int val = (*it).first;
    idx++;
    while (idx < where) {
        val++;
        idx++;
    }
    return Domain::iterator(mRanges.begin(), mRanges.end(), it, val);
}

Domain Domain::slice(int from, int to) {
    std::set<std::pair<int, int>> ranges;
    Domain::iterator fromIt = at(from);
    Domain::iterator toIt = at(to);
    if (fromIt == end()) { return Domain(); }
    std::set<std::pair<int, int>>::iterator it = fromIt.mRangeIterator;
    std::set<std::pair<int, int>>::iterator hint = ranges.begin();
    if (it != toIt.mRangeIterator) {
        hint = ranges.insert({*fromIt, (*it).second}).first;
    }
    it++;
    for (; it != toIt.mRangeIterator; it++) {
        hint = ranges.insert(hint, *it);
    }
    if (toIt != end() && *toIt != (*it).first) {
        ranges.insert(hint, {(*it).first, *toIt});
    }
    return Domain(ranges);
}