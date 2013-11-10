#pragma once

#include <vector>
#include <assert.h>
#include <iostream>

template<typename T>
class TraitsSentinel;

// Class representing binary heap with user specified key type and value type
template <typename TKey, typename TValue>
class BinaryHeap {
    public:
        typedef int TIndex;
        typedef TKey TKeyType;
        typedef TValue TValueType;

        BinaryHeap(std::size_t max_size);
        TValue extractMax();
        const TValue& getMax() const;
        void insert(TKey key, TValue value);
        void replace(TIndex prev_idx, TKey new_key, TValue value);
        void debugPrint() const;
        const std::vector<TKey>& getKeys() const;
        const std::vector<TValue>& getValues() const;
        int getSize() const;
        int getMaxSize() const;
    private:
        void heapify(TIndex idx);
        TIndex increaseKey(TKey new_key, TIndex idx);
        void swapElements(TIndex a, TIndex b);
        TIndex getParentIndex(TIndex idx);
        TIndex getLeftIndex(TIndex idx);
        TIndex getRightIndex(TIndex idx);

        std::size_t heap_size_;
        std::size_t max_heap_size_;
        std::vector<TKey> keys_;
        std::vector<TValue> values_;
};

template<typename TKey, typename TValue>
BinaryHeap<TKey, TValue>::BinaryHeap(std::size_t max_size) : 
    heap_size_(0),
    max_heap_size_(max_size),
    keys_(max_heap_size_),
    values_(max_heap_size_) {}

template<typename TKey, typename TValue>
TValue BinaryHeap<TKey, TValue>::extractMax() {
    assert(heap_size_ >= 1);
    
    swapElements(0, heap_size_ - 1);
    TValue max = values_[heap_size_ - 1];
    // debug mode
    values_[heap_size_ - 1] = TValue();
    keys_[heap_size_ - 1] = TKey();
    heapify(0);
    heap_size_--;
    return max;
}

template<typename TKey, typename TValue>
const TValue& BinaryHeap<TKey, TValue>::getMax() const{
    assert(heap_size_ >= 1);
    
    return values[0];
}

template<typename TKey, typename TValue>
typename BinaryHeap<TKey, TValue>::TIndex BinaryHeap<TKey, TValue>::getParentIndex(TIndex idx) {
    assert(idx < max_heap_size_);
    if(idx <= 0) return -1;
    return (idx - 1) >> 1;
}

template<typename TKey, typename TValue>
typename BinaryHeap<TKey, TValue>::TIndex BinaryHeap<TKey, TValue>::getLeftIndex(TIndex idx) {
    assert(idx >= 0 && idx < heap_size_);
    return (idx << 1) + 1;
}

template<typename TKey, typename TValue>
typename BinaryHeap<TKey, TValue>::TIndex BinaryHeap<TKey, TValue>::getRightIndex(TIndex idx) {
    assert(idx >= 0 && idx < heap_size_);
    return (idx << 1) + 2;
}

template<typename TKey, typename TValue>
void BinaryHeap<TKey, TValue>::swapElements(TIndex a, TIndex b) {
    assert(a >= 0 && a < max_heap_size_);
    assert(b >= 0 && b < max_heap_size_);
    using std::swap;
    swap(keys_[a], keys_[b]);
    swap(values_[a], values_[b]);
}

template<typename TKey, typename TValue>
int BinaryHeap<TKey, TValue>::getSize() const {
    assert(heap_size_ >= 0 && heap_size_ <= max_heap_size_);
    return heap_size_;
}

template<typename TKey, typename TValue>
int BinaryHeap<TKey, TValue>::getMaxSize() const {
    assert(max_heap_size_ >= 0 && heap_size_ >= 0 && heap_size_ <= max_heap_size_);
    return max_heap_size_;
}

template<typename TKey, typename TValue>
const std::vector<TKey>& BinaryHeap<TKey, TValue>::getKeys() const {
    assert(keys_.size() == max_heap_size_ && keys_.size() == values_.size());
    return keys_;
}

template<typename TKey, typename TValue>
const std::vector<TValue>& BinaryHeap<TKey, TValue>::getValues() const {
    assert(values_.size() == max_heap_size_ && keys_.size() == values_.size());
    return values_;
}

template<typename TKey, typename TValue>
void BinaryHeap<TKey, TValue>::heapify(TIndex idx) {
    assert(idx >= 0 && idx < heap_size_);

    TIndex left_idx = getLeftIndex(idx);
    TIndex right_idx = getRightIndex(idx);
    TIndex max_idx = idx;

    if (left_idx <= heap_size_ && keys_[left_idx] > keys_[idx]) {
        max_idx = left_idx;
    }
    if (right_idx <= heap_size_ && keys_[right_idx] > keys_[idx]) {
        max_idx = left_idx;
    }
    if (max_idx != idx) {
        swapElements(idx, max_idx);
        heapify(max_idx);
    }
}

template<typename TKey, typename TValue>
void BinaryHeap<TKey, TValue>::insert(TKey key, TValue value) {
    assert(heap_size_ + 1 <= max_heap_size_);

    keys_[heap_size_] = TraitsSentinel<TKey>::MINUS_SENTINEL;
    TIndex new_idx = increaseKey(key, heap_size_);
    assert(new_idx >= 0 && new_idx < max_heap_size_);

    values_[new_idx] = value;
    ++heap_size_;
}

template<typename TKey, typename TValue>
void BinaryHeap<TKey, TValue>::replace(TIndex prev_idx, 
                                       TKey new_key, 
                                       TValue value) {
    assert(prev_idx >= 0 && prev_idx < heap_size_);

    TIndex idx = increaseKey(new_key, prev_idx);
    assert(idx >= 0 && idx < max_heap_size_);

    values_[idx] = value;
}

template<typename TKey, typename TValue>
typename BinaryHeap<TKey, TValue>::TIndex BinaryHeap<TKey, TValue>::increaseKey(TKey new_key, TIndex idx) {
    assert(idx >= 0 && idx < max_heap_size_);
    assert(keys_[idx] <= new_key);

    keys_[idx] = new_key;
    TIndex parent_idx = getParentIndex(idx);

    while(idx > 0 && keys_[parent_idx] < keys_[idx]) {
        swapElements(idx, parent_idx);
        idx = parent_idx;
        parent_idx = getParentIndex(idx);
    }

    return idx;
}

template<typename TKey, typename TValue>
void BinaryHeap<TKey, TValue>::debugPrint() const {
    for(auto k_it = keys_.begin(); k_it != keys_.end(); ++k_it) {
        std::cout << *k_it << '\t';
    }
    std::cout << "\n";
    for(auto v_it = values_.begin(); v_it != values_.end(); ++v_it) {
        std::cout << *v_it << '\t';
    }
    std::cout << "\n";
}

