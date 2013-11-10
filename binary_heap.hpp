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
        BinaryHeap(std::size_t max_size);
        TValue extractMax();
        void insert(TKey key, TValue value);
        void replace(TIndex prev_idx, TKey new_key, TValue value);
        void debugPrint() const;
    private:
        void heapify(TIndex idx);
        TIndex increaseKey(TKey new_key, TIndex idx);
        TValue& operator[] (TIndex idx);
        void swapElements(TIndex a, TIndex b);
        TIndex getParentIndex(TIndex idx);
        TIndex getLeftIndex(TIndex idx);
        TIndex getRightIndex(TIndex idx);

        std::size_t heap_size;
        std::size_t max_heap_size;
        std::vector<TKey> keys;
        std::vector<TValue> values;
};

template<typename TKey, typename TValue>
BinaryHeap<TKey, TValue>::BinaryHeap(std::size_t max_size) : 
    heap_size(0),
    max_heap_size(max_size),
    keys(max_heap_size),
    values(max_heap_size) {}

template<typename TKey, typename TValue>
TValue BinaryHeap<TKey, TValue>::extractMax() {
    assert(heap_size >= 1);
    
    swapElements(0, heap_size - 1);
    TValue max = values[heap_size - 1];
    // debug mode
    values[heap_size - 1] = TValue();
    keys[heap_size - 1] = TKey();
    heap_size--;
    heapify(0);
    return max;
}

template<typename TKey, typename TValue>
typename BinaryHeap<TKey, TValue>::TIndex BinaryHeap<TKey, TValue>::getParentIndex(TIndex idx) {
    if(idx <= 0) return -1;
    return (idx - 1) >> 1;
}

template<typename TKey, typename TValue>
typename BinaryHeap<TKey, TValue>::TIndex BinaryHeap<TKey, TValue>::getLeftIndex(TIndex idx) {
    return (idx << 1) + 1;
}

template<typename TKey, typename TValue>
typename BinaryHeap<TKey, TValue>::TIndex BinaryHeap<TKey, TValue>::getRightIndex(TIndex idx) {
    return (idx << 1) + 2;
}

template<typename TKey, typename TValue>
void BinaryHeap<TKey, TValue>::swapElements(TIndex a, TIndex b) {
    using std::swap;
    swap(keys[a], keys[b]);
    swap(values[a], values[b]);
}

template<typename TKey, typename TValue>
void BinaryHeap<TKey, TValue>::heapify(TIndex idx) {
    TIndex left_idx = getLeftIndex(idx);
    TIndex right_idx = getRightIndex(idx);
    TIndex max_idx = idx;
    if (left_idx <= heap_size && keys[left_idx] > keys[idx]) {
        max_idx = left_idx;
    }
    if (right_idx <= heap_size && keys[right_idx] > keys[idx]) {
        max_idx = left_idx;
    }
    if (max_idx != idx) {
        swapElements(idx, max_idx);
        heapify(max_idx);
    }
}

template<typename TKey, typename TValue>
void BinaryHeap<TKey, TValue>::insert(TKey key, TValue value) {
    assert(heap_size + 1 <= max_heap_size);
    keys[heap_size] = TraitsSentinel<TKey>::MINUS_SENTINEL;
    TIndex new_idx = increaseKey(key, heap_size);
    assert(new_idx >= 0 && new_idx < max_heap_size);
    values[new_idx] = value;
    ++heap_size;
}

template<typename TKey, typename TValue>
void BinaryHeap<TKey, TValue>::replace(TIndex prev_idx, 
                                       TKey new_key, 
                                       TValue value) {
    assert(prev_idx >= 0 && prev_idx < heap_size);
    TIndex idx = increaseKey(new_key, prev_idx);
    assert(idx >= 0 && idx < max_heap_size);
    values[idx] = value;
}

template<typename TKey, typename TValue>
typename BinaryHeap<TKey, TValue>::TIndex BinaryHeap<TKey, TValue>::increaseKey(TKey new_key, TIndex idx) {
    assert(idx >= 0 && idx < max_heap_size);
    assert(keys[idx] <= new_key);
    keys[idx] = new_key;
    TIndex parent_idx = getParentIndex(idx);
    while(idx > 0 && keys[parent_idx] < keys[idx]) {
        swapElements(idx, parent_idx);
        idx = parent_idx;
        parent_idx = getParentIndex(idx);
    }
    return idx;
}

template<typename TKey, typename TValue>
void BinaryHeap<TKey, TValue>::debugPrint() const {
    for(auto k_it = keys.begin(); k_it != keys.end(); ++k_it) {
        std::cout << *k_it << '\t';
    }
    std::cout << "\n";
    for(auto v_it = values.begin(); v_it != values.end(); ++v_it) {
        std::cout << *v_it << '\t';
    }
    std::cout << "\n";
}
