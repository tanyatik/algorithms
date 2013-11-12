#include <iostream>
#include <algorithm>
#include <vector>
#include <assert.h>

template <typename TElem>
class BinaryHeap {
    public:
        typedef long long TIndex;
        typedef TElem TElemType;

        BinaryHeap(std::size_t max_size, 
                   std::function <bool(TElem&, TElem&)> comparator,
                   std::function <TElem()> sentinel);
        TIndex removeTop();
        const TElem& getTop() const;
        TIndex insert(TElem elem);
        TElem &operator [] (TIndex index);
        const TElem &operator [] (TIndex index) const;
        TIndex replace(TIndex prev_idx, TElem new_elem);
        TIndex remove(TIndex idx);
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
        std::function <bool(TElem&, TElem&)> comparator_;
        std::function <TElem()> sentinel_;
};

template<typename TElem>
BinaryHeap<TElem>::BinaryHeap(std::size_t max_size, 
                              std::function<bool(TElem&, TElem&)> comparator,
                              std::function<TElem()> sentinel) : 
    heap_size_(0),
    max_heap_size_(max_size),
    elems_(max_heap_size_),
    comparator_(comparator),
    sentinel_(sentinel) {}

template<typename TElem>
const TElem& BinaryHeap<TElem>::getTop() const{
    assert(heap_size_ >= 1);
   
    return elems_[0];
}

template<typename TElem>
typename BinaryHeap<TElem>::TIndex BinaryHeap<TElem>::getParentIndex(TIndex idx) {
    assert(idx < max_heap_size_);
    if(idx <= 0) return -1;
    return (idx - 1) >> 1;
}

template<typename TElem>
typename BinaryHeap<TElem>::TIndex BinaryHeap<TElem>::getLeftIndex(TIndex idx) {
    assert(idx == 0 || (idx > 0 && idx <= heap_size_));
    return (idx << 1) + 1;
}

template<typename TElem>
typename BinaryHeap<TElem>::TIndex BinaryHeap<TElem>::getRightIndex(TIndex idx) {
    assert(idx == 0 || (idx > 0 && idx <= heap_size_));
    return (idx << 1) + 2;
}

template<typename TElem>
void BinaryHeap<TElem>::swapElements(TIndex a, TIndex b) {
    assert(a >= 0 && a < max_heap_size_);
    assert(b >= 0 && b < max_heap_size_);
    using std::swap;
    swap_(elems_[a], elems_[b]);
}

template<typename TElem>
unsigned int BinaryHeap<TElem>::getSize() const {
    assert(heap_size_ >= 0 && heap_size_ <= max_heap_size_);
    return heap_size_;
}

template<typename TElem>
TElem& BinaryHeap<TElem>::operator [] (TIndex idx) {
    assert(idx >= 0 && idx <= heap_size_);
    // just as an experiment
    return const_cast<TElem&>(static_cast<const BinaryHeap<TElem>&>(*this)[idx]);
}

template<typename TElem>
const TElem& BinaryHeap<TElem>::operator [] (TIndex idx) const {
    assert(idx >= 0 && idx <= heap_size_);
    return elems_[idx];
}

template<typename TElem>
void BinaryHeap<TElem>::heapifyDown(TIndex idx) {
    assert(idx == 0 || (idx > 0 && idx <= heap_size_));

    TIndex left_idx = getLeftIndex(idx);
    TIndex right_idx = getRightIndex(idx);
    TIndex max_idx = idx;

    if (left_idx < heap_size_ && comparator_(elems_[max_idx], elems_[left_idx])) {
        max_idx = left_idx;
    }
    if (right_idx < heap_size_ && comparator_(elems_[max_idx], elems_[right_idx])) {
        max_idx = right_idx;
    }
    if (max_idx != idx) {
        swapElements(idx, max_idx);
        assert(max_idx >= 0 && max_idx < heap_size_);
        heapifyDown(max_idx);
    }
}

template<typename TElem>
typename BinaryHeap<TElem>::TIndex BinaryHeap<TElem>::insert(TElem elem) {
    assert(heap_size_ + 1 <= max_heap_size_);

    elems_[heap_size_] = sentinel_();
    TIndex new_idx = increaseKey(elem, heap_size_);
    assert(new_idx >= 0 && new_idx < max_heap_size_);

    ++heap_size_;
    return new_idx;
}

template<typename TElem>
typename BinaryHeap<TElem>::TIndex BinaryHeap<TElem>::replace(TIndex prev_idx, 
                                       TElem new_elem) {
    assert(prev_idx >= 0 && prev_idx < heap_size_);

    TIndex idx = increaseKey(new_elem, prev_idx);
    assert(idx >= 0 && idx < max_heap_size_);

    return idx;
}

template<typename TElem>
typename BinaryHeap<TElem>::TIndex BinaryHeap<TElem>::removeTop() {
    assert(heap_size_ >= 1);
    
    deleteElement(0);
    heapifyDown(0);
    return heap_size_;
}

template<typename TElem>
typename BinaryHeap<TElem>::TIndex BinaryHeap<TElem>::remove(TIndex idx) {
    assert(idx >= 0 && idx < heap_size_);
    assert(heap_size_ >= 1);
    
    deleteElement(idx);

    idx = heapifyUp(idx);
    heapifyDown(idx);
    return heap_size_;
}

template<typename TElem>
typename BinaryHeap<TElem>::TIndex BinaryHeap<TElem>::increaseKey(TElem new_elem, TIndex idx) {
    assert(idx >= 0 && idx < max_heap_size_);
    assert(comparator_(elems_[idx], new_elem));

    elems_[idx] = new_elem;
    idx = heapifyUp(idx);
    return idx;
}

template<typename TElem>
void BinaryHeap<TElem>::deleteElement(TIndex idx) {
    swapElements(idx, heap_size_ - 1);
    --heap_size_;
}

template<typename TElem>
void BinaryHeap<TElem>::debugPrint() const {
    for(auto k_it = elems_.begin(); k_it != elems_.end(); ++k_it) {
        std::cout << *k_it;
        std::cout << '\t';
    }
    std::cout << "\n";
}

template<typename TElem>
typename BinaryHeap<TElem>::TIndex BinaryHeap<TElem>::heapifyUp(TIndex idx) {
    TIndex parent_idx = getParentIndex(idx);

    while(idx > 0 && comparator_(elems_[parent_idx], elems_[idx])) {
        swapElements(idx, parent_idx);
        idx = parent_idx;
        parent_idx = getParentIndex(idx);
    }
    return idx;
}

struct Number;

typedef BinaryHeap<Number *> Heap;

struct Number {
    int number;
    Heap *heap_ptr;
    int heap_idx;

    Number(int number, Heap *heap_ptr, int heap_idx) :
        number(number),
        heap_ptr(heap_ptr),
        heap_idx(heap_idx) {}
};

bool comparatorLess(Number *one, Number *other) {
    return (one->number < other->number) || 
           (one->number == other->number && one->heap_idx < other->heap_idx);
}

bool comparatorGreat(Number *one, Number *other) {
    return (one->number > other->number) || 
           (one->number == other->number && one->heap_idx > other->heap_idx);
}

Number *getMinusSentinel() {
    static Number number(-1000000, nullptr, -100000);
    return &number;
}

Number *getSentinel() {
    static Number number(100000, nullptr, 100000);
    return &number;
}

void checkHeap(BinaryHeap<Number *> *heap) {
    for (int i = 0; i < heap->getSize(); ++i) {
        assert((*heap)[i]->heap_ptr == heap);
        assert((*heap)[i]->heap_idx == i);
    }
}

void swap_(Number *&one, Number *&two) {
    std::swap(one, two);
    std::swap(one->heap_idx, two->heap_idx);
}

void insertHeap(BinaryHeap<Number *> *heap, Number *elem) {
    elem->heap_idx = heap->getSize();
    int idx = heap->insert(elem);
    (*heap)[idx]->heap_ptr = heap;
    (*heap)[idx]->heap_idx = idx;
}

void kOrderStatistics(int k, 
                      const std::vector<int> &array, 
                      const std::vector<char> &moves, std::vector<int> *result) {

    int left = 0, right = 0;
    BinaryHeap<Number *> less_heap(k, comparatorLess, getMinusSentinel); // max
    BinaryHeap<Number *> greater_heap(array.size(), comparatorGreat, getSentinel); // min
    std::vector<Number *> tmp_elements(array.size(), nullptr);
    tmp_elements[0] = new Number(array[0], &less_heap, 0);
    insertHeap(&less_heap, tmp_elements[0]);

    for (auto it = moves.begin(); it < moves.end(); ++it) {
        if (*it == 'L') {
            left++;

            Number *rem_elem = tmp_elements[left - 1];
            BinaryHeap<Number *> *t_heap = rem_elem->heap_ptr;
            int rem_idx = t_heap->remove(rem_elem->heap_idx);
            (*t_heap)[rem_idx] = nullptr;
    
            if(less_heap.getSize() < k && greater_heap.getSize() > 0) {
                int gt_idx = greater_heap.removeTop();
                Number *element = greater_heap[gt_idx];
                greater_heap[gt_idx] = nullptr;

                insertHeap(&less_heap, element);
            }
        }
        else if (*it == 'R') {
            right++;
            Number *elem = new Number(array[right], nullptr, 0);
            tmp_elements[right] = elem;

            if(less_heap.getSize() < k) {
                insertHeap(&less_heap, elem);

            } else if (comparatorLess(less_heap.getTop(), elem)) {
                insertHeap(&greater_heap, elem);
            } else {
                Number *max_lt = less_heap.getTop();
                int max_lt_idx = less_heap.removeTop();
                less_heap[max_lt_idx] = nullptr;

                insertHeap(&greater_heap, max_lt);
                insertHeap(&less_heap, elem);
            } 
        }
        result->push_back(less_heap.getSize() < k ? -1 : less_heap.getTop()->number);

        checkHeap(&less_heap);
        checkHeap(&greater_heap);
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
