#pragma once
#include <iterator>
#include <set>

/**
 * Representation of a domain as a single continuous range of integers.
 * Represented as a half-open range, i.e. [1,5)
 * Since it is a single continuous range, this WILL NOT work with GAC, but it will work with BC.
 * More efficient than GACRangeDomain, since it's simpler.
 */
class BCRangeDomain
{
public:
    class iterator;
    using const_iterator = iterator;
    BCRangeDomain() : mRange({0, 0}) { };
    BCRangeDomain(const std::set<int> &vals);
    BCRangeDomain(std::initializer_list<int> vals);
    BCRangeDomain& operator=(const BCRangeDomain &d) {
        mRange = d.mRange;
        return *this;
    }
    friend bool operator==(const BCRangeDomain &a, const BCRangeDomain &b) {
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
    BCRangeDomain slice(int from, int to);
    int size() const {
        return mRange.second - mRange.first;
    }

    std::pair<int, int> mRange = {0,0};
private:
};

/**
 * an iterator for this class so that we can iterate over all values of the domain
 */
class BCRangeDomain::iterator : public std::iterator<std::bidirectional_iterator_tag, int>
{
public:
    iterator& operator=(const iterator &it) {
        mVal=it.mVal;
        return *this;
    }
    friend bool operator==(const iterator& a, const iterator& b) {
        return a.mVal == b.mVal;// && *(a.mBCRangeDomain) == *(b.mBCRangeDomain);
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
    int mVal = 0;
    friend class BCRangeDomain;
};

inline BCRangeDomain::iterator BCRangeDomain::iterator::operator++(int) {
    iterator copy = *this;
    ++mVal;
    return copy;
}

inline BCRangeDomain::iterator BCRangeDomain::iterator::operator++() {
    ++mVal;
    return *this;
}

inline BCRangeDomain::iterator BCRangeDomain::iterator::operator--(int) {
    iterator copy = *this;
    --mVal;
    return copy;
}

inline BCRangeDomain::iterator BCRangeDomain::iterator::operator--() {
    --mVal;
    return *this;
}