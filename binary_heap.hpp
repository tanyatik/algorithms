#pragma once

#include <vector>
#include <assert.h>
#include <iostream>
#include <functional>

template <typename TElement,
          typename TComparator,
          typename MoveCallback>
class BinaryHeap;

struct EmptyCallback {
    template<typename TElement, typename TIndex>
    void operator () (const TElement &, 
                      TIndex) {}
};


template <typename TElement,
          typename TComparator = std::less<TElement>,
          typename MoveCallback = EmptyCallback>
class BinaryHeap {
    public:
        typedef long long TIndex;
        typedef TElement TElementType;

        // Removes element on the top of the heap
        void pop();
        const TElement& getTop() const; 
        void insert(const TElement& element);
        // Removes element which is located by index, keeping all heap properties
        void removeElementByIndex (TIndex index);
        
        // Number of elements kept in the heap
        unsigned int getSize() const;

    private:
        bool compareElements(const TElement &one, const TElement &other) const;

        void heapifyDown(TIndex index);
        TIndex heapifyUp(TIndex index);

        void swapElements(TIndex a, TIndex b);

        TIndex getParentIndex(TIndex index);
        TIndex getLeftIndex(TIndex index);
        TIndex getRightIndex(TIndex index);

        // Helper function -- swaps element with index 'index' 
        // with last element of the heap,
        // and then removes last element
        void deleteElement(TIndex index);

        std::vector<TElement> elements_;
};

template<typename TElement, typename TComparator, typename TMoveCallback>
bool BinaryHeap<TElement, TComparator, TMoveCallback>::compareElements(const TElement &one, 
                                                        const TElement &other) const {
    return TComparator() (one, other);
}

template<typename TElement, typename TComparator, typename TMoveCallback>
const TElement &BinaryHeap<TElement, TComparator, TMoveCallback>::getTop() const {
    return elements_[0];
}

template<typename TElement, typename TComparator, typename TMoveCallback>
typename BinaryHeap<TElement, TComparator, TMoveCallback>::TIndex BinaryHeap<TElement, TComparator, TMoveCallback>::getParentIndex
        (TIndex index) {
    if (index <= 0) return -1;
    return (index - 1) >> 1;
}

template<typename TElement, typename TComparator, typename TMoveCallback>
typename BinaryHeap<TElement, TComparator, TMoveCallback>::TIndex BinaryHeap<TElement, TComparator, TMoveCallback>::getLeftIndex
        (TIndex index) {
    return (index << 1) + 1;
}

template<typename TElement, typename TComparator, typename TMoveCallback>
typename BinaryHeap<TElement, TComparator, TMoveCallback>::TIndex BinaryHeap<TElement, TComparator, TMoveCallback>::getRightIndex
        (TIndex index) {
    return (index << 1) + 2;
}

template<typename TElement, typename TComparator, typename TMoveCallback>
void BinaryHeap<TElement, TComparator, TMoveCallback>::swapElements(TIndex a, TIndex b) {
    using std::swap;
    swap(elements_[a], elements_[b]);
    TMoveCallback()(elements_[a], a);
    TMoveCallback()(elements_[b], b);
}

template<typename TElement, typename TComparator, typename TMoveCallback>
unsigned int BinaryHeap<TElement, TComparator, TMoveCallback>::getSize() const {
    return elements_.size();
}

template<typename TElement, typename TComparator, typename TMoveCallback>
void BinaryHeap<TElement, TComparator, TMoveCallback>::heapifyDown(TIndex index) {
    TIndex left_index = getLeftIndex(index);
    TIndex right_index = getRightIndex(index);
    TIndex max_index = index;

    if (left_index < getSize() && compareElements(elements_[max_index], elements_[left_index])) {
        max_index = left_index;
    }
    if (right_index < getSize() && compareElements(elements_[max_index], elements_[right_index])) {
        max_index = right_index;
    }
    if (max_index != index) {
        swapElements(index, max_index);
        heapifyDown(max_index);
    }
}

template<typename TElement, typename TComparator, typename TMoveCallback>
void BinaryHeap<TElement, TComparator, TMoveCallback>::insert(const TElement &element) {
    elements_.push_back(element);

    int heap_size = elements_.size() - 1;
    elements_[heap_size] = element;
    TMoveCallback()(elements_[heap_size], heap_size);

    heapifyUp(heap_size);
}

template<typename TElement, typename TComparator, typename TMoveCallback>
void BinaryHeap<TElement, TComparator, TMoveCallback>::pop() {
    deleteElement(0);
    heapifyDown(0);
}

template<typename TElement, typename TComparator, typename TMoveCallback>
void BinaryHeap<TElement, TComparator, TMoveCallback>::removeElementByIndex(TIndex index) {
    deleteElement(index);
    
    if (index < getSize()) {
        index = heapifyUp(index);
        heapifyDown(index);
    }
}

template<typename TElement, typename TComparator, typename TMoveCallback>
void BinaryHeap<TElement, TComparator, TMoveCallback>::deleteElement(TIndex index) {
    swapElements(index, getSize() - 1);
    elements_.erase(elements_.end() - 1);
    //elements_[getSize() - 1] = TElement();
}

template<typename TElement, typename TComparator, typename TMoveCallback>
typename BinaryHeap<TElement, TComparator, TMoveCallback>::TIndex BinaryHeap<TElement, TComparator, TMoveCallback>::heapifyUp
            (TIndex index) {
    TIndex parent_index = getParentIndex(index);

    while (index > 0 && compareElements(elements_[parent_index], elements_[index])) {
        swapElements(index, parent_index);
        index = parent_index;
        parent_index = getParentIndex(index);
    }
    return index;
}
