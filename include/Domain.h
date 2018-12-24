#pragma once
#include <iterator>
#include <set>

/**
 * Representation of a domain. Hopefully replacement for set of ints.
 * A sorted set of pairs of integers. The pairs represent disjoint ranges of integers that make up the domain;
 * the pair (a, b) represents the range [a, b).
 */
class Domain
{
public:
    class iterator : public std::iterator<std::bidirectional_iterator_tag, int>
    {
    public:
        friend bool operator==(const iterator& a, const iterator& b) {
            return a.mVal == b.mVal && a.mRangeIterator == b.mRangeIterator;
        };
        friend bool operator!=(const iterator& a, const iterator& b) {
            return !(a == b);
        };
        // prefix
        iterator operator++() {
            iterator copy = this;
            mVal++;
            if (mVal == (*mRangeIterator).second) {
                mRangeIterator++;
                mVal = mRangeIterator != mEnd ? (*mRangeIterator).first : -1;
            }
            return copy;
        };
        // postfix
        iterator operator++(int) {
            mVal++;
            if (mVal == (*mRangeIterator).second) {
                mRangeIterator++;
                mVal = mRangeIterator != mEnd ? (*mRangeIterator).first : -1;
            }
            return this;
        };
        iterator operator--() {
            iterator copy = this;
            mVal--;
            if (mVal < (*mRangeIterator).first) {
                mRangeIterator--;
                mVal = std::next(mRangeIterator) != mBegin ? (*mRangeIterator).second : -1;
            }
            return copy;
        };
        iterator operator--(int) {
            mVal--;
            if (mVal < (*mRangeIterator).first) {
                mRangeIterator--;
                mVal = std::next(mRangeIterator) != mBegin ? (*mRangeIterator).second : -1;
            }
            return this;
        };
        int operator*() {
            return mVal;
        };
    private:
        iterator(std::set<std::pair<int, int>>::iterator begin,
                std::set<std::pair<int, int>>::iterator end,
                std::set<std::pair<int, int>>::iterator rangeIt,
                int val)
                : mBegin(begin), mEnd(end), mRangeIterator(rangeIt), mVal(val) { }
        std::set<std::pair<int, int>>::iterator mBegin;
        std::set<std::pair<int, int>>::iterator mEnd;
        std::set<std::pair<int, int>>::iterator mRangeIterator;
        int mVal;
        friend class Domain;
    };
    using const_iterator = iterator;

    Domain(std::set<int> &&vals);
    Domain(std::set<std::pair<int, int>> &&ranges) : mRanges(ranges) { };
    friend bool operator==(const Domain &a, const Domain &b) {
        return a.mRanges == b.mRanges;
    }
    iterator begin();
    iterator end();
    iterator insert(int val);
    iterator insert(std::pair<int, int> range);
    iterator insert(Domain &&d);
    iterator erase(iterator &it);
    iterator find(int val);
    iterator at(int where);
    Domain slice(int from, int to);
    int size() {
        int count = 0;
        for (std::pair<int, int> &&range : mRanges) {
            count += range.second - range.first;
        }
        return count;
    }

private:
    std::set<std::pair<int, int>> mRanges;
};