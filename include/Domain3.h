#pragma once
#include <iterator>
#include <set>

/**
 * Representation of a domain. Hopefully replacement for set of ints.
 * A sorted set of pairs of integers. The pairs represent disjoint ranges of integers that make up the domain;
 * the pair (a, b) represents the range [a, b).
 */
class Domain3
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

        friend class Domain3;
    };

    using const_iterator = iterator;
    Domain3() : mRanges({}), mSize(0) { }
    Domain3(const Domain3 &d) : mRanges(d.mRanges), mSize(d.mSize) { }
    Domain3(const std::set<std::pair<int, int>>& ranges) : mRanges(ranges) {
        for (auto& range : mRanges) {
            mSize += range.second - range.first;
        }
    }
    Domain3(const std::initializer_list<int>& vals) {
        for (int v : vals) {
            insert(v);
        }
    }
    template<typename T>
    Domain3(T& vals) {
        for (int v : vals) {
            insert(v);
        }
    }
    Domain3& operator=(const Domain3 &d) {
        mRanges = d.mRanges;
        mSize = d.mSize;
        return *this;
    }
    friend bool operator==(const Domain3 &a, const Domain3 &b) {
        return a.mSize == b.mSize && a.mRanges == b.mRanges;
    }
    iterator begin() const;
    iterator end() const;
    std::reverse_iterator<iterator> rbegin() const;
    std::reverse_iterator<iterator> rend() const;
    iterator insert(int val);
    void insert(std::pair<int, int> range);
    void insert(Domain3& d);
    template <typename T>
    void insert(T start, T finish);
    iterator erase(iterator it);
    iterator find(int val) const;
    iterator at(int where) const;
    Domain3 slice(int from, int to);
    std::size_t size() const {
        return mSize;
    }

private:
    std::set<std::pair<int, int>> mRanges = {};
    std::size_t mSize = 0;
};

inline void Domain3::iterator::inc() {
    if (++mVal == (*mRangeIt).second && mRangeIt != --(mRanges->end())) {
        mVal = (*++mRangeIt).first;
    }
}

inline void Domain3::iterator::dec() {
    if (--mVal < (*mRangeIt).first && mRangeIt != mRanges->begin()) {
        mVal = (*--mRangeIt).second-1;
    }
}

inline Domain3::iterator Domain3::iterator::operator++(int) {
    iterator copy = *this;
    inc();
    return copy;
}

inline Domain3::iterator Domain3::iterator::operator++() {
    inc();
    return *this;
}

inline Domain3::iterator Domain3::iterator::operator--(int) {
    iterator copy = *this;
    dec();
    return copy;
}

inline Domain3::iterator Domain3::iterator::operator--() {
    dec();
    return *this;
}