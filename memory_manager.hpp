#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>


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
        /*if (*element) {
            (*element)->debugPrint();
            std::cout << '\t';
        }*/
        std::cout << *element << '\t';
    }
    std::cout << "\n";
}

// MemoryBlock
class MemoryBlock {
public:
    unsigned int size;
    unsigned int position;
    MemoryBlock *p_prev;
    MemoryBlock *p_next;
    int free_heap_index; // stores -1 if block is occupied

    MemoryBlock();
    MemoryBlock(unsigned int size, 
                unsigned int position, 
                long long free_heap_index);
    void debugPrint() const;
};

MemoryBlock::MemoryBlock() :
    size(0),
    position(0),
    p_prev(nullptr), 
    p_next(nullptr),
    free_heap_index(0) {}

MemoryBlock::MemoryBlock(unsigned int size, unsigned int position, long long free_heap_index) :
    size(size),
    position(position),
    p_prev(nullptr), 
    p_next(nullptr),
    free_heap_index(free_heap_index) {}

void MemoryBlock::debugPrint() const {
    std::cout << "[p" << position << ":s" << size << ':' 
        << free_heap_index  << "]";
}

bool memoryBlocksComparator (const MemoryBlock *one, const MemoryBlock *other) {
    return ((one->size < other->size) || 
            (one->size == other->size && one->position > other->position));
}

void swap (MemoryBlock *&one, MemoryBlock *&other) {
    std::swap(one->free_heap_index, other->free_heap_index);
    std::swap(one, other);
}

std::ostream &operator << (std::ostream &stream, MemoryBlock *block) {
    if (block) {
        block->debugPrint();
    }
    return stream;
}

// Memory manager
class MemoryManager {
    public:
        MemoryManager(unsigned memory_cells_number, unsigned int max_query_number);
        ~MemoryManager();

        long long allocate(unsigned int block_size);
        void deallocate(unsigned int query_number);
        void debugPrint() const;
    private:
        unsigned int memory_cells_number_;
        unsigned int query_number_;
        unsigned int query_counter_;
        std::vector<MemoryBlock*> occupied_blocks_;
         
        BinaryHeap<MemoryBlock*> free_blocks_;
        MemoryBlock *head_;

        void connectBlocks(MemoryBlock *one, MemoryBlock *other);
        bool isOccupied(const MemoryBlock *block) const;

        void releaseOccupiedBlock(unsigned int query_number);
        void setOccupiedBlock(MemoryBlock *biggest_block);
        void releaseFreeBlock(MemoryBlock **block_pointer);
        MemoryBlock *createNewFreeBlock(unsigned int size, unsigned int position);
        MemoryBlock *createNewOccupiedBlock(unsigned int size, unsigned int position);
        void setOccupiedBlockFree(MemoryBlock *occupied_block, unsigned int query_number);
        void setFreeBlockOccupied(MemoryBlock *occupied_block);
};

MemoryManager::MemoryManager(unsigned int memory_cells_number, unsigned int query_number) :
    memory_cells_number_(memory_cells_number),
    query_number_(query_number),
    query_counter_(0),
    occupied_blocks_(query_number, nullptr),
    free_blocks_(query_number, memoryBlocksComparator),
    head_(new MemoryBlock(0, 0, -1)) {

    MemoryBlock *free_block = createNewFreeBlock(memory_cells_number, 0);
    connectBlocks(head_, free_block);
}

MemoryManager::~MemoryManager() {
    for (unsigned int idx = 0; idx < query_number_; ++idx) {
        if (occupied_blocks_[idx] != nullptr) {
            delete occupied_blocks_[idx];
            occupied_blocks_[idx] = nullptr;
        }
    }
    
    for (auto free_block : free_blocks_.getElements()) {
        if (free_block != nullptr) {
            delete free_block;
        }
    }
    delete head_;
}

void MemoryManager::releaseOccupiedBlock(unsigned int query_number) {
    assert(query_number < query_number_);
    MemoryBlock *ptr = occupied_blocks_[query_number];
    delete ptr;
    occupied_blocks_[query_number] = nullptr;
}

void MemoryManager::setOccupiedBlock(MemoryBlock *biggest_block) {
    occupied_blocks_[query_counter_] = biggest_block;
}

void MemoryManager::releaseFreeBlock(MemoryBlock **block_pointer) {
    free_blocks_.removeElementByIndex((*block_pointer)->free_heap_index);
    delete *block_pointer;
    *block_pointer = nullptr;
}

MemoryBlock *MemoryManager::createNewFreeBlock(unsigned int size, unsigned int position) {
    // hack (we know index in the heap of the new element)
    MemoryBlock *new_block = new MemoryBlock(size, position, free_blocks_.getSize());
    free_blocks_.insert(new_block);
    return new_block;
}

MemoryBlock *MemoryManager::createNewOccupiedBlock(unsigned int size, unsigned int position) {
    MemoryBlock *new_block = new MemoryBlock(size, position, -1);
    occupied_blocks_[query_counter_] = new_block;
    return new_block;
}

void MemoryManager::setFreeBlockOccupied(MemoryBlock *free_block) {
    free_blocks_.removeElementByIndex(free_block->free_heap_index);
    free_block->free_heap_index = -1;

    setOccupiedBlock(free_block);
}

void MemoryManager::setOccupiedBlockFree(MemoryBlock *occupied_block, unsigned int query_number) {
    occupied_block->free_heap_index = free_blocks_.getSize();
    free_blocks_.insert(occupied_block);
    occupied_blocks_[query_number] = nullptr;
}

/*
void MemoryManager::Block(MemoryBlock *block_pointer) {
    occupied_blocks_[query_number_] = nullptr;
}
*/
long long MemoryManager::allocate(unsigned int block_size) {
    long long position = -1;
    MemoryBlock *biggest_block = free_blocks_.getSize() > 0 ? free_blocks_.getTop() : nullptr;

    if (!biggest_block || biggest_block->size < block_size) {
        setOccupiedBlock(nullptr);

    } else if (biggest_block->size == block_size) {
        position = biggest_block->position;
        // ... |         free        | ...
        //                ||     
        //                \/    
        // ... |         occ         | ...     occ for occupied here
        MemoryBlock *biggest_block = free_blocks_.getTop();
        setFreeBlockOccupied(biggest_block);
    } else if (biggest_block->size > block_size) {
        position = biggest_block->position;
        // ... |         free        | ...
        //                 ||     
        //                 \/    
        // ... | occ  |      free    | ...
        MemoryBlock *left_block_to_connect = biggest_block->p_prev;
        MemoryBlock *right_block_to_connect = biggest_block->p_next;

        MemoryBlock *new_occupied_block = createNewOccupiedBlock(block_size, biggest_block->position);
        MemoryBlock *new_free_block = createNewFreeBlock(biggest_block->size - block_size, 
                                                     biggest_block->position + block_size);

        connectBlocks(left_block_to_connect, new_occupied_block);
        connectBlocks(new_occupied_block, new_free_block);
        connectBlocks(new_free_block, right_block_to_connect);

        releaseFreeBlock(&biggest_block);
    }

    ++query_counter_;
    return position;
}

void MemoryManager::deallocate(unsigned int query_number) {
    MemoryBlock *occupied = occupied_blocks_[query_number]; // occ -- occupied block to free
    if (!occupied) {
        ++query_counter_;
        return;
    }
    if (isOccupied(occupied->p_prev) && isOccupied(occupied->p_next)) {
        // ... | occ  |  occ | occ | ...
        //               ||     
        //               \/    
        // ... | occ  | free | occ | ...
        MemoryBlock *block = occupied_blocks_[query_number];
        setOccupiedBlockFree(block, query_number);
    } else if (!isOccupied(occupied->p_prev) && isOccupied(occupied->p_next)) {
        // ... | free |  occ | occ | ...
        //               ||     
        //               \/    
        // ... |   free      | occ | ...
        MemoryBlock *free_left = occupied->p_prev;
        MemoryBlock *new_free_block = createNewFreeBlock(free_left->size + occupied->size, 
                                                     free_left->position);

        connectBlocks(free_left->p_prev, new_free_block);
        connectBlocks(new_free_block, occupied->p_next);

//        std::cout << "BEFORE RELEASE \n";
//        free_blocks_.debugPrint();
//        std::cout << "BLOCK \n";
//        free_left->debugPrint();
        releaseFreeBlock(&free_left);

        releaseOccupiedBlock(query_number);
    } else if (isOccupied(occupied->p_prev) && !isOccupied(occupied->p_next)) {
        // ... | occ  |  occ | free | ...
        //               ||     
        //               \/    
        // ... | occ  |    free     | ...
        MemoryBlock *free_right = occupied->p_next;
        MemoryBlock *new_free_block = createNewFreeBlock(free_right->size + occupied->size, 
                                                     occupied->position);

        connectBlocks(occupied->p_prev, new_free_block);
        connectBlocks(new_free_block, occupied->p_next->p_next);

        releaseFreeBlock(&free_right);
        releaseOccupiedBlock(query_number);
    } else {
        // ... | free  |  occ | free | ...
        //                 ||     
        //                 \/    
        // ... |         free        | ...
        MemoryBlock *free_left = occupied->p_prev;
        MemoryBlock *free_right = occupied->p_next;

        MemoryBlock *new_free_block = createNewFreeBlock(free_left->size 
                                                        + occupied->size 
                                                        + free_right->size,
                                                        free_left->position);

        connectBlocks(free_left->p_prev, new_free_block); 
        connectBlocks(new_free_block, free_right->p_next);
        
        releaseFreeBlock(&free_left);
        releaseFreeBlock(&free_right);
        
        releaseOccupiedBlock(query_number);
    }
    ++query_counter_;
}

void MemoryManager::connectBlocks(MemoryBlock *one, MemoryBlock *other) {
    if (one) {
        one->p_next = other;
    }
    if (other) {
        other->p_prev = one;
    }
}

bool MemoryManager::isOccupied(const MemoryBlock *ptr) const {
    if (ptr) {
        return ptr->free_heap_index == -1;
    } else {
        return true;
    }
}

void MemoryManager::debugPrint() const {
    std::cout << "MEMORY MANAGER \n";
    MemoryBlock *ptr = head_->p_next;
    while (ptr) {
        ptr->debugPrint();
        std::cout << ' ';
        ptr = ptr->p_next;
    }
    std::cout << std::endl;
    std::cout << "HEAP \n";
    free_blocks_.debugPrint();
    std::cout << std::endl;
    std::cout << "LIST \n";
    for (auto ptr : occupied_blocks_) {
        if (ptr) {
            ptr->debugPrint();
        } else {
            std::cout << "null ";
        }
    }
    std::cout << std::endl << std::endl; 
}
