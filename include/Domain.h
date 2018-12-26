#pragma once
#include <iterator>
#include <set>
#include "types.h"

/**
 * Representation of a domain. Hopefully replacement for set of ints.
 * A sorted set of pairs of integers. The pairs represent disjoint ranges of integers that make up the domain;
 * the pair (a, b) represents the range [a, b).
 */
class Domain
{
public:
    class iterator;
    using const_iterator = iterator;
    Domain() : mRanges({}) { };
    Domain(const std::set<int> &vals);
//    Domain(std::initializer_list<int> vals) { Domain(std::set<int>(vals)); };
    Domain(const std::set<std::pair<int, int>> &ranges) : mRanges(ranges) { };
//    Domain(std::initializer_list<std::pair<int, int>> vals) { Domain(std::set<std::pair<int, int>>(vals)); };
    friend bool operator==(Domain &a, Domain &b) {
        return a.mRanges == b.mRanges;
    }
    iterator begin() const;
    iterator end() const;
    std::reverse_iterator<iterator> rbegin() const;
    std::reverse_iterator<iterator> rend() const;
    iterator insert(int val);
    iterator insert(std::pair<int, int> range);
    void insert(const Domain &d);
    iterator erase(iterator &it);
    iterator find(int val) const;
    iterator at(int where) const;
    Domain slice(int from, int to);
    int size() const {
        int count = 0;
        for (auto&& range : mRanges) {
            count += range.second - range.first;
        }
        return count;
    }

private:
    std::set<std::pair<int, int>> mRanges;
};

// inlining to hopefully be slightly faster idk
class Domain::iterator : public std::iterator<std::bidirectional_iterator_tag, int>
{
public:
    friend bool operator==(const iterator& a, const iterator& b) {
        return a.mVal == b.mVal && a.mRangeIterator == b.mRangeIterator;
    };
    friend bool operator!=(const iterator& a, const iterator& b) {
        return !(a == b);
    };
    // postfix
    iterator operator++(int);
    // prefix
    iterator operator++();
    iterator operator--(int);
    iterator operator--();
    // not inlined, bc compiler optimizations were causing strange behavior
    int& operator*();
private:
//    iterator() : mBegin({}), mEnd({}), mRangeIterator({}), mVal(-1) { }
    iterator(std::set<std::pair<int, int>>::iterator beginIt,
            std::set<std::pair<int, int>>::iterator endIt,
            std::set<std::pair<int, int>>::iterator rangeIt,
    int val)
    : mBegin(beginIt), mEnd(endIt), mRangeIterator(rangeIt), mVal(val) { }
    std::set<std::pair<int, int>>::iterator mBegin;
    std::set<std::pair<int, int>>::iterator mEnd;
    std::set<std::pair<int, int>>::iterator mRangeIterator;
    int mVal;
    friend class Domain;
};

inline Domain::iterator Domain::iterator::operator++(int) {
    iterator copy = *this;
    ++mVal;
    if (mVal == (*mRangeIterator).second) {
        ++mRangeIterator;
        mVal = mRangeIterator != mEnd ? (*mRangeIterator).first : -1;
    }
    return copy;
}

inline Domain::iterator Domain::iterator::operator++() {
    ++mVal;
    if (mVal == (*mRangeIterator).second) {
        ++mRangeIterator;
        mVal = mRangeIterator != mEnd ? (*mRangeIterator).first : -1;
    }
    return *this;
}

inline Domain::iterator Domain::iterator::operator--(int) {
    iterator copy = *this;
    --mVal;
    if (mRangeIterator == mEnd || mVal < (*mRangeIterator).first) {
        --mRangeIterator;
        mVal = (*mRangeIterator).second-1;
    }
    return copy;
}

inline Domain::iterator Domain::iterator::operator--() {
    --mVal;
    if (mRangeIterator == mEnd || mVal < (*mRangeIterator).first) {
        --mRangeIterator;
        mVal = (*mRangeIterator).second-1;
    }
    return *this;
}