#include <iostream>
#include <algorithm>
#include <vector>
#include <assert.h>

// Class representing binary heap with user specified elem type and value type
template <typename TElem, typename TComparator>
class BinaryHeap {
    public:
        typedef long long TIndex;
        typedef TElem TElemType;

        BinaryHeap(std::size_t max_size);
        TElem removeTop();
        const TElem& getTop() const;
        TIndex insert(TElem elem);
        TElem& operator [] (TIndex index);
        const TElem& operator [] (TIndex index) const;
        TIndex replace(TIndex prev_idx, TElem new_elem);
        void remove(TIndex idx);
        void debugPrint() const;
        unsigned int getSize() const;
    private:
        void heapifyDown(TIndex idx);
        TIndex heapifyUp(TIndex idx);
        TIndex increaseKey(TElem new_elem, TIndex idx);
        void swapElements(TIndex a, TIndex b);
        TIndex getParentIndex(TIndex idx);
        TIndex getLeftIndex(TIndex idx);
        TIndex getRightIndex(TIndex idx);
        void deleteElement(TIndex idx);

        unsigned int heap_size_;
        unsigned int max_heap_size_;
        std::vector<TElem> elems_;
};

template<typename TElem, typename TComparator>
BinaryHeap<TElem, TComparator>::BinaryHeap(std::size_t max_size) : 
    heap_size_(0),
    max_heap_size_(max_size),
    elems_(max_heap_size_) {}

template<typename TElem, typename TComparator>
const TElem& BinaryHeap<TElem, TComparator>::getTop() const{
    assert(heap_size_ >= 1);
    
    return elems_[0];
}

template<typename TElem, typename TComparator>
typename BinaryHeap<TElem, TComparator>::TIndex BinaryHeap<TElem, TComparator>::getParentIndex(TIndex idx) {
    assert(idx < max_heap_size_);
    if(idx <= 0) return -1;
    return (idx - 1) >> 1;
}

template<typename TElem, typename TComparator>
typename BinaryHeap<TElem, TComparator>::TIndex BinaryHeap<TElem, TComparator>::getLeftIndex(TIndex idx) {
    assert(idx == 0 || (idx > 0 && idx < heap_size_));
    return (idx << 1) + 1;
}

template<typename TElem, typename TComparator>
typename BinaryHeap<TElem, TComparator>::TIndex BinaryHeap<TElem, TComparator>::getRightIndex(TIndex idx) {
    assert(idx == 0 || (idx > 0 && idx < heap_size_));
    return (idx << 1) + 2;
}

template<typename TElem, typename TComparator>
void BinaryHeap<TElem, TComparator>::swapElements(TIndex a, TIndex b) {
    assert(a >= 0 && a < max_heap_size_);
    assert(b >= 0 && b < max_heap_size_);
    using std::swap;
    swap(elems_[a], elems_[b]);
}

template<typename TElem, typename TComparator>
unsigned int BinaryHeap<TElem, TComparator>::getSize() const {
    assert(heap_size_ >= 0 && heap_size_ <= max_heap_size_);
    return heap_size_;
}

template<typename TElem, typename TComparator>
TElem& BinaryHeap<TElem, TComparator>::operator [] (TIndex idx) {
    assert(idx >= 0 && idx < heap_size_);
    // just as an experiment
    return const_cast<TElem&>(static_cast<const BinaryHeap<TElem, TComparator>&>(*this)[idx]);
}

template<typename TElem, typename TComparator>
const TElem& BinaryHeap<TElem, TComparator>::operator [] (TIndex idx) const {
    assert(idx >= 0 && idx < heap_size_);
    return elems_[idx];
}

template<typename TElem, typename TComparator>
void BinaryHeap<TElem, TComparator>::heapifyDown(TIndex idx) {
    assert(idx == 0 || (idx > 0 && idx < heap_size_));

    TIndex left_idx = getLeftIndex(idx);
    TIndex right_idx = getRightIndex(idx);
    TIndex max_idx = idx;
    TComparator comparator;

    if (left_idx < heap_size_ && comparator(elems_[max_idx], elems_[left_idx])) {
        max_idx = left_idx;
    }
    if (right_idx < heap_size_ && comparator(elems_[max_idx], elems_[right_idx])) {
        max_idx = right_idx;
    }
    if (max_idx != idx) {
        swapElements(idx, max_idx);
        assert(max_idx >= 0 && max_idx < heap_size_);
        heapifyDown(max_idx);
    }
}

template<typename TElem, typename TComparator>
typename BinaryHeap<TElem, TComparator>::TIndex BinaryHeap<TElem, TComparator>::insert(TElem elem) {
    assert(heap_size_ + 1 <= max_heap_size_);

    elems_[heap_size_] = TComparator::getMinusSentinel();
    TIndex new_idx = increaseKey(elem, heap_size_);
    assert(new_idx >= 0 && new_idx < max_heap_size_);

    ++heap_size_;
    return new_idx;
}

template<typename TElem, typename TComparator>
typename BinaryHeap<TElem, TComparator>::TIndex BinaryHeap<TElem, TComparator>::replace(TIndex prev_idx, 
                                       TElem new_elem) {
    assert(prev_idx >= 0 && prev_idx < heap_size_);

    TIndex idx = increaseKey(new_elem, prev_idx);
    assert(idx >= 0 && idx < max_heap_size_);

    return idx;
}

template<typename TElem, typename TComparator>
TElem BinaryHeap<TElem, TComparator>::removeTop() {
    assert(heap_size_ >= 1);
    
    TElem max_elem = elems_[0];
    deleteElement(0);
    heapifyDown(0);
    return max_elem;
}

template<typename TElem, typename TComparator>
void BinaryHeap<TElem, TComparator>::remove(TIndex idx) {
    assert(idx >= 0 && idx < heap_size_);
    assert(heap_size_ >= 1);
    
    deleteElement(idx);

    idx = heapifyUp(idx);
    heapifyDown(idx);
}

template<typename TElem, typename TComparator>
typename BinaryHeap<TElem, TComparator>::TIndex BinaryHeap<TElem, TComparator>::increaseKey(TElem new_elem, TIndex idx) {
    TComparator comparator;
    assert(idx >= 0 && idx < max_heap_size_);
    assert(comparator(elems_[idx], new_elem));

    elems_[idx] = new_elem;
    idx = heapifyUp(idx);
    return idx;
}

template<typename TElem, typename TComparator>
void BinaryHeap<TElem, TComparator>::deleteElement(TIndex idx) {
    swapElements(idx, heap_size_ - 1);
    elems_[heap_size_ - 1] = TElem();
    --heap_size_;
}

template<typename TElem, typename TComparator>
void BinaryHeap<TElem, TComparator>::debugPrint() const {
    for(auto k_it = elems_.begin(); k_it != elems_.end(); ++k_it) {
        std::cout << *k_it;
        std::cout << '\t';
    }
    std::cout << "\n";
}

template<typename TElem, typename TComparator>
typename BinaryHeap<TElem, TComparator>::TIndex BinaryHeap<TElem, TComparator>::heapifyUp(TIndex idx) {
    TIndex parent_idx = getParentIndex(idx);
    TComparator comparator;

    while(idx > 0 && comparator(elems_[parent_idx], elems_[idx])) {
        swapElements(idx, parent_idx);
        idx = parent_idx;
        parent_idx = getParentIndex(idx);
    }
    return idx;
}

struct Number {
    int number;
    int order;

    struct ComparatorLess {
        bool operator () (Number one, Number other) {
            return (one.number < other.number) || 
                   (one.number == other.number && one.order < other.order);
        }
        static Number getMinusSentinel() {
            return Number{-1000000000, -1000000000};
        }
    };

    struct ComparatorGreat {
        bool operator () (Number one, Number other) {
            return (one.number > other.number) || 
                   (one.number == other.number && one.order > other.order);
        }
        static Number getMinusSentinel() {
            return Number{1000000000, 1000000000};
        }
    };
};

template<typename TBinaryHeap>
void removeExpired(int order, TBinaryHeap *heap) {
    while (heap->getSize() > 0 && heap->getTop().order < order) {
        heap->removeTop();
    }
};

void kOrderStatistics(int k, const std::vector<int> &array, const std::vector<char> &moves, std::vector<int> *result) {
    BinaryHeap<Number, Number::ComparatorLess> less_heap(k);
    BinaryHeap<Number, Number::ComparatorGreat> greater_heap(array.size());

    int left = 0, right = 0;
    std::vector<int> less_heap_indexes(array.size());
    less_heap_indexes[0] = less_heap.insert(Number{array[0], 0});

    for (auto it = moves.begin(); it < moves.end(); ++it) {
        if (*it == 'L') {
            left++;
            int idx = less_heap_indexes[left - 1];
            // bad array
            less_heap.remove(idx);

            removeExpired(left, &less_heap);
            removeExpired(left, &greater_heap);

            if(less_heap.getSize() < k && greater_heap.getSize() > 0) {
                less_heap.insert(greater_heap.removeTop());
            }
        }
        else if (*it == 'R') {
            right++;
            Number elem{array[right], right};
            Number::ComparatorLess comparator;

            if(less_heap.getSize() < k) {
                int idx = less_heap.insert(elem);
                less_heap_indexes[right] = idx;

            } else if (comparator(less_heap.getTop(), elem)) {
                greater_heap.insert(elem);

            } else {
                Number max_left = less_heap.removeTop();
                greater_heap.insert(max_left);
                int idx = less_heap.insert(elem);
                less_heap_indexes[right] = idx;
            } 
        }
        result->push_back(less_heap.getSize() < k ? -1 : less_heap.getTop().number);
    }
}


void readInput(int *k, std::vector<int> *vector, std::vector<char> *moves) {
    int vector_size, moves_number;
    std::cin >> vector_size >> moves_number >> *k;
    vector->resize(vector_size);
    moves->resize(moves_number);

    for (std::vector<int>::iterator it = vector->begin(); it < vector->end(); ++it) {
        std::cin >> *it;
    }

    for (std::vector<char>::iterator it = moves->begin(); it < moves->end(); ++it) {
        std::cin >> *it;
    } 
}

int main() {
    int k;
    std::vector<int> array;
    std::vector<char> moves;
    std::vector<int> result;
    
    readInput(&k, &array, &moves);
    kOrderStatistics(k, array, moves, &result);
    for_each(result.begin(), result.end(), [](int it){std::cout << it << std::endl;});

    return 0;
}
