#pragma once
#include <iterator>
#include <set>

/**
 * Representation of a domain as an ordered set of disjoint ranges, i.e. ([1,3), [4,5), [9,17)}.
 * Works with GAC and BC, since the domain can handle gaps.
 * Less efficient than BCRangeDomain, but more efficient than a set of ints.
 */
class GACRangeDomain
{
public:
    class iterator : public std::iterator<std::bidirectional_iterator_tag, int>
    {
    public:
        iterator()=delete;
        iterator(const iterator& it) : mRanges(it.mRanges), mVal(it.mVal), mRangeIt(it.mRangeIt) { }
        iterator& operator=(const iterator &it) {
            mRanges=it.mRanges;
            mVal=it.mVal;
            mRangeIt=it.mRangeIt;
            return *this;
        }
        friend bool operator==(const iterator& a, const iterator& b) {
            return a.mVal == b.mVal && a.mRangeIt == b.mRangeIt;
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
        iterator(const std::set<std::pair<int, int>>* ranges,
                 int val,
                 std::set<std::pair<int, int>>::const_iterator it) : mRanges(ranges), mVal(val), mRangeIt(it) { }
        void inc();
        void dec();

        const std::set<std::pair<int, int>>* mRanges = nullptr;
        int mVal;
        std::set<std::pair<int, int>>::const_iterator mRangeIt;

        friend class GACRangeDomain;
    };

    using const_iterator = iterator;
    GACRangeDomain() : mRanges({}), mSize(0) { }
    GACRangeDomain(const GACRangeDomain &d) : mRanges(d.mRanges), mSize(d.mSize) { }
    GACRangeDomain(const std::set<std::pair<int, int>>& ranges) : mRanges(ranges) {
        for (auto& range : mRanges) {
            mSize += range.second - range.first;
        }
    }
    GACRangeDomain(const std::initializer_list<int>& vals) {
        for (int v : vals) {
            insert(v);
        }
    }
    template<typename T>
    GACRangeDomain(T& vals) {
        for (int v : vals) {
            insert(v);
        }
    }
    GACRangeDomain& operator=(const GACRangeDomain &d) {
        mRanges = d.mRanges;
        mSize = d.mSize;
        return *this;
    }
    friend bool operator==(const GACRangeDomain &a, const GACRangeDomain &b) {
        return a.mSize == b.mSize && a.mRanges == b.mRanges;
    }
    iterator begin() const;
    iterator end() const;
    std::reverse_iterator<iterator> rbegin() const;
    std::reverse_iterator<iterator> rend() const;
    iterator insert(int val);
    void insert(std::pair<int, int> range);
    void insert(GACRangeDomain& d);
    template <typename T>
    void insert(T start, T finish);
    iterator erase(iterator it);
    iterator find(int val) const;
    iterator at(int where) const;
    GACRangeDomain slice(int from, int to);
    std::size_t size() const {
        return mSize;
    }

private:
    std::set<std::pair<int, int>> mRanges = {};
    std::size_t mSize = 0;
};

inline void GACRangeDomain::iterator::inc() {
    if (++mVal == (*mRangeIt).second && mRangeIt != --(mRanges->end())) {
        mVal = (*++mRangeIt).first;
    }
}

inline void GACRangeDomain::iterator::dec() {
    if (--mVal < (*mRangeIt).first && mRangeIt != mRanges->begin()) {
        mVal = (*--mRangeIt).second-1;
    }
}

inline GACRangeDomain::iterator GACRangeDomain::iterator::operator++(int) {
    iterator copy = *this;
    inc();
    return copy;
}

inline GACRangeDomain::iterator GACRangeDomain::iterator::operator++() {
    inc();
    return *this;
}

inline GACRangeDomain::iterator GACRangeDomain::iterator::operator--(int) {
    iterator copy = *this;
    dec();
    return copy;
}

inline GACRangeDomain::iterator GACRangeDomain::iterator::operator--() {
    dec();
    return *this;
}