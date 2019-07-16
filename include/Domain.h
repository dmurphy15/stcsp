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
    class iterator;
    using const_iterator = iterator;
    Domain() : mRange({0, 0}) { };
    Domain(const std::set<int> &vals);
    Domain(std::initializer_list<int> vals);
    Domain operator=(const Domain &d) {
        mRange = d.mRange;
    }
    friend bool operator==(const Domain &a, const Domain &b) {
        return a.mRange == b.mRange || (a.size() == 0 && b.size() == 0);
    }
    iterator begin() const;
    iterator end() const;
    std::reverse_iterator<iterator> rbegin() const;
    std::reverse_iterator<iterator> rend() const;
    iterator insert(int val);
    void insert(iterator start, iterator finish);
    void insert(std::set<int>::iterator start, std::set<int>::iterator finish);
    iterator erase(iterator &it);
    iterator find(int val) const;
    iterator at(int where) const;
    Domain slice(int from, int to);
    int size() const {
        return mRange.second - mRange.first;
    }

    std::pair<int, int> mRange;
private:
};

// inlining to hopefully be slightly faster idk
class Domain::iterator : public std::iterator<std::bidirectional_iterator_tag, int>
{
public:
    iterator& operator=(const iterator &it) { mVal=it.mVal; }
    friend bool operator==(const iterator& a, const iterator& b) {
        return a.mVal == b.mVal;// && *(a.mDomain) == *(b.mDomain);
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
    iterator() : mVal(0) { }
    iterator(int val) :
        mVal(val) { }
    int mVal;
    int mIndex = 0;
    int mVals[64];
    friend class Domain;
};

inline Domain::iterator Domain::iterator::operator++(int) {
    iterator copy = *this;
    ++mVal;
    return copy;
}

inline Domain::iterator Domain::iterator::operator++() {
    ++mVal;
    return *this;
}

inline Domain::iterator Domain::iterator::operator--(int) {
    iterator copy = *this;
    --mVal;
    return copy;
}

inline Domain::iterator Domain::iterator::operator--() {
    --mVal;
    return *this;
}