#pragma once

#include <vector>
#include <assert.h>
#include <iostream>
#include <functional>

template <typename TElement>
class BinaryHeap {
    public:
        typedef long long TIndex;
        typedef TElement TElementType;

        explicit BinaryHeap(std::size_t max_size);
        BinaryHeap(std::size_t max_size, 
                   std::function<bool(TElement, TElement)> comparator);

        void pop();
        const TElement& getTop() const;
        
        void insert(const TElement& element);

        void removeElementByIndex (TIndex index);
        
        void debugPrint() const;
        const std::vector<TElement>& getElements() const;
        unsigned int getSize() const;

    private:
        bool compareElements(const TElement &one, const TElement &other) const;

        void heapifyDown(TIndex index);
        TIndex heapifyUp(TIndex index);

        TIndex increaseKey(TElement new_element, TIndex index);
        void swapElements(TIndex a, TIndex b);

        TIndex getParentIndex(TIndex index);
        TIndex getLeftIndex(TIndex index);
        TIndex getRightIndex(TIndex index);

        void deleteElement(TIndex index);

        unsigned int heap_size_;
        unsigned int max_heap_size_;
        std::vector<TElement> elements_;
        std::function<bool(TElement, TElement)> comparator_;
};

template<typename TElement>
BinaryHeap<TElement>::BinaryHeap(std::size_t max_size) : 
    heap_size_(0),
    max_heap_size_(max_size),
    elements_(max_heap_size_),
    comparator_([] (const TElement &one, const TElement &other) {
        return one < other;
    }) {}

template<typename TElement>
BinaryHeap<TElement>::BinaryHeap(std::size_t max_size, 
                                 std::function<bool(TElement, TElement)> comparator) : 
    heap_size_(0),
    max_heap_size_(max_size),
    elements_(max_heap_size_),
    comparator_(comparator) {}

template<typename TElement>
bool BinaryHeap<TElement>::compareElements(const TElement &one, const TElement &other) const {
    return comparator_(one, other);
}

template<typename TElement>
const TElement &BinaryHeap<TElement>::getTop() const {
    assert(heap_size_ >= 1);
    
    return elements_[0];
}

template<typename TElement>
const std::vector<TElement> &BinaryHeap<TElement>::getElements() const {
    return elements_;
}

template<typename TElement>
typename BinaryHeap<TElement>::TIndex BinaryHeap<TElement>::getParentIndex(TIndex index) {
    assert(index < max_heap_size_);
    if(index <= 0) return -1;
    return (index - 1) >> 1;
}

template<typename TElement>
typename BinaryHeap<TElement>::TIndex BinaryHeap<TElement>::getLeftIndex(TIndex index) {
    assert(index == 0 || (index > 0 && index < heap_size_));
    return (index << 1) + 1;
}

template<typename TElement>
typename BinaryHeap<TElement>::TIndex BinaryHeap<TElement>::getRightIndex(TIndex index) {
    assert(index == 0 || (index > 0 && index < heap_size_));
    return (index << 1) + 2;
}

template<typename TElement>
void BinaryHeap<TElement>::swapElements(TIndex a, TIndex b) {
    assert(a >= 0 && a < max_heap_size_);
    assert(b >= 0 && b < max_heap_size_);
    using std::swap;
    swap(elements_[a], elements_[b]);
}

template<typename TElement>
unsigned int BinaryHeap<TElement>::getSize() const {
    assert(heap_size_ <= max_heap_size_);
    return heap_size_;
}

template<typename TElement>
void BinaryHeap<TElement>::heapifyDown(TIndex index) {
    assert(index == 0 || (index > 0 && index < heap_size_));

    TIndex left_index = getLeftIndex(index);
    TIndex right_index = getRightIndex(index);
    TIndex max_index = index;

    if (left_index < heap_size_ && compareElements(elements_[max_index], elements_[left_index])) {
        max_index = left_index;
    }
    if (right_index < heap_size_ && compareElements(elements_[max_index], elements_[right_index])) {
        max_index = right_index;
    }
    if (max_index != index) {
        swapElements(index, max_index);
        assert(max_index >= 0 && max_index < heap_size_);
        heapifyDown(max_index);
    }
}

template<typename TElement>
void BinaryHeap<TElement>::insert(const TElement &element) {
    assert(heap_size_ + 1 <= max_heap_size_);

    elements_[heap_size_] = element;
    TIndex new_index = heapifyUp(heap_size_);
    assert(new_index >= 0 && new_index <= heap_size_);

    ++heap_size_;
}

template<typename TElement>
void BinaryHeap<TElement>::pop() {
    assert(heap_size_ >= 1);
    
    deleteElement(0);
    heapifyDown(0);
}

template<typename TElement>
void BinaryHeap<TElement>::removeElementByIndex(TIndex index) {
    assert(index >= 0 && index < heap_size_);
    assert(heap_size_ >= 1);
    
    deleteElement(index);
    
    if (index < heap_size_) {
        index = heapifyUp(index);
        assert(index >= 0 && index <= heap_size_);
        heapifyDown(index);
    }
}

template<typename TElement>
typename BinaryHeap<TElement>::TIndex BinaryHeap<TElement>::increaseKey(TElement new_element, TIndex index) {
    assert(index >= 0 && index < max_heap_size_);
    assert(compareElements(elements_[index], new_element));

    elements_[index] = new_element;
    index = heapifyUp(index);
    return index;
}

template<typename TElement>
void BinaryHeap<TElement>::deleteElement(TIndex index) {
    swapElements(index, heap_size_ - 1);
    elements_[heap_size_ - 1] = TElement();
    --heap_size_;
}

template<typename TElement>
typename BinaryHeap<TElement>::TIndex BinaryHeap<TElement>::heapifyUp(TIndex index) {
    TIndex parent_index = getParentIndex(index);

    while(index > 0 && compareElements(elements_[parent_index], elements_[index])) {
        swapElements(index, parent_index);
        index = parent_index;
        parent_index = getParentIndex(index);
    }
    return index;
}

template<typename TElement>
void BinaryHeap<TElement>::debugPrint() const {
    for(auto element = elements_.begin(); element != elements_.end(); ++element) {
        element.debugPrint();
        std::cout << '\t';
    }
    std::cout << "\n";
}


