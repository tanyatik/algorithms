#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

template <typename TElement>
class BinaryHeap {
    public:
        typedef long long TIndex;
        typedef TElement TElementType;
        typedef std::function < bool (TElement, TElement) > TComparator;

        explicit BinaryHeap(std::size_t max_size);
        BinaryHeap(std::size_t max_size, 
                   TComparator comparator);

        // Removes element on the top of the heap
        void pop();
        const TElement& getTop() const; 
        void insert(const TElement& element);
        // Removes element which is located by index, keeping all heap properties
        void removeElementByIndex (TIndex index);
        
        const std::vector<TElement>& getElements() const;
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

        unsigned int heap_size_;
        unsigned int max_heap_size_;
        std::vector<TElement> elements_;
        TComparator comparator_;
};

class MemoryBlock {
private:
    unsigned int size_;
    unsigned int position_;
    // All blocks are connnected in two-forked linked list
    // Each block has to know its predecessor and successor,
    // to complete merging of two free blocks placed side by side
    // Previous and next block can be either in the heap of free blocks,
    // or in the array of occupied blocks
    MemoryBlock *previous_block_;
    MemoryBlock *next_block_;

    // Stores index in heap of free blocks,
    // or OCCUPIED_BLOCK if this block is not in this heap
    int free_heap_index_;

public:
    static const int OCCUPIED_BLOCK = -1;

    MemoryBlock();
    MemoryBlock(unsigned int size, 
                unsigned int position, 
                long long free_heap_index);

    unsigned int getSize() const;

    unsigned int getPosition() const;

    MemoryBlock *getPreviousBlock() const;
    void setPreviousBlock(MemoryBlock *previous_block);

    MemoryBlock *getNextBlock() const;
    void setNextBlock(MemoryBlock *next_block);

    int getFreeHeapIndex() const;
    void setFreeHeapIndex(int free_heap_index);
};

class MemoryManager {
    public:
        MemoryManager(unsigned memory_cells_number, unsigned int max_request_number);
        ~MemoryManager();

        long long allocate(unsigned int block_size);
        void deallocate(unsigned int request_number);

    private:
        unsigned int memory_cells_number_;
        unsigned int request_total_number_;
        unsigned int request_counter_;

        // Indices in the array are number of queries
        // Each array element with index 'i' stores either a occupied block,
        // which was allocated as result of request number 'i',
        // or nullptr to indicate that no block was allocated on request number 'i' 
        std::vector<MemoryBlock*> occupied_blocks_;
        // Stores all free blocks 
        // There can be no free blocks placed side-by-side
        // When free block is occupied, it is deleted from the heap
        BinaryHeap<MemoryBlock*> free_blocks_;

        void connectBlocks(MemoryBlock *previous, MemoryBlock *next);
        bool isOccupied(const MemoryBlock *block) const;

        MemoryBlock *createNewFreeBlock(unsigned int size, unsigned int position);
        MemoryBlock *createNewOccupiedBlock(unsigned int size, unsigned int position);

        void releaseOccupiedBlock(unsigned int request_number);
        void releaseFreeBlock(MemoryBlock **block_pointer);
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
                                 TComparator comparator) : 
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
    return elements_[0];
}

template<typename TElement>
const std::vector<TElement> &BinaryHeap<TElement>::getElements() const {
    return elements_;
}

template<typename TElement>
typename BinaryHeap<TElement>::TIndex BinaryHeap<TElement>::getParentIndex(TIndex index) {
    if (index <= 0) return -1;
    return (index - 1) >> 1;
}

template<typename TElement>
typename BinaryHeap<TElement>::TIndex BinaryHeap<TElement>::getLeftIndex(TIndex index) {
    return (index << 1) + 1;
}

template<typename TElement>
typename BinaryHeap<TElement>::TIndex BinaryHeap<TElement>::getRightIndex(TIndex index) {
    return (index << 1) + 2;
}

template<typename TElement>
void BinaryHeap<TElement>::swapElements(TIndex a, TIndex b) {
    using std::swap;
    swap(elements_[a], elements_[b]);
}

template<typename TElement>
unsigned int BinaryHeap<TElement>::getSize() const {
    return heap_size_;
}

template<typename TElement>
void BinaryHeap<TElement>::heapifyDown(TIndex index) {
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
        heapifyDown(max_index);
    }
}

template<typename TElement>
void BinaryHeap<TElement>::insert(const TElement &element) {
    elements_[heap_size_] = element;
    heapifyUp(heap_size_);

    ++heap_size_;
}

template<typename TElement>
void BinaryHeap<TElement>::pop() {
    deleteElement(0);
    heapifyDown(0);
}

template<typename TElement>
void BinaryHeap<TElement>::removeElementByIndex(TIndex index) {
    deleteElement(index);
    
    if (index < heap_size_) {
        index = heapifyUp(index);
        heapifyDown(index);
    }
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

    while (index > 0 && compareElements(elements_[parent_index], elements_[index])) {
        swapElements(index, parent_index);
        index = parent_index;
        parent_index = getParentIndex(index);
    }
    return index;
}

MemoryBlock::MemoryBlock() :
    size_(0),
    position_(0),
    previous_block_(nullptr), 
    next_block_(nullptr),
    free_heap_index_(0) {}

MemoryBlock::MemoryBlock(unsigned int size, unsigned int position, long long free_heap_index) :
    size_(size),
    position_(position),
    previous_block_(nullptr), 
    next_block_(nullptr),
    free_heap_index_(free_heap_index) {}

unsigned int MemoryBlock::getSize() const {
    return size_;
}

unsigned int MemoryBlock::getPosition() const {
    return position_;
}

MemoryBlock *MemoryBlock::getPreviousBlock() const {
    return previous_block_;
}

MemoryBlock *MemoryBlock::getNextBlock() const {
    return next_block_;
}

void MemoryBlock::setPreviousBlock(MemoryBlock *previous_block) {
    previous_block_ = previous_block;
}

void MemoryBlock::setNextBlock(MemoryBlock *next_block) {
    next_block_ = next_block;
}

int MemoryBlock::getFreeHeapIndex() const {
    return free_heap_index_;
}

void MemoryBlock::setFreeHeapIndex(int free_heap_index) {
    free_heap_index_ = free_heap_index;
}


bool memoryBlocksComparator (const MemoryBlock *one, const MemoryBlock *other) {
    return ((one->getSize() < other->getSize()) || 
            (one->getSize() == other->getSize() && one->getPosition() > other->getPosition()));
}

void swap (MemoryBlock *&one, MemoryBlock *&other) {
    int temp = one->getFreeHeapIndex();
    one->setFreeHeapIndex(other->getFreeHeapIndex());
    other->setFreeHeapIndex(temp);

    std::swap(one, other);
}

MemoryManager::MemoryManager(unsigned int memory_cells_number, unsigned int request_total_number) :
    memory_cells_number_(memory_cells_number),
    request_total_number_(request_total_number),
    request_counter_(0),
    occupied_blocks_(request_total_number, nullptr),
    free_blocks_(request_total_number, memoryBlocksComparator) {

    createNewFreeBlock(memory_cells_number, 0);
}

MemoryManager::~MemoryManager() {
    for (unsigned int idx = 0; idx < request_total_number_; ++idx) {
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
}

void MemoryManager::releaseOccupiedBlock(unsigned int request_number) {
    MemoryBlock *ptr = occupied_blocks_[request_number];
    delete ptr;
    occupied_blocks_[request_number] = nullptr;
}

void MemoryManager::releaseFreeBlock(MemoryBlock **block_pointer) {
    free_blocks_.removeElementByIndex((*block_pointer)->getFreeHeapIndex());
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
    MemoryBlock *new_block = new MemoryBlock(size, position, MemoryBlock::OCCUPIED_BLOCK);
    occupied_blocks_[request_counter_] = new_block;
    return new_block;
}

long long MemoryManager::allocate(unsigned int block_size) {
    long long position = -1;
    MemoryBlock *biggest_block = free_blocks_.getSize() > 0 ? free_blocks_.getTop() : nullptr;

    if (!biggest_block || biggest_block->getSize() < block_size) {
        occupied_blocks_[request_counter_] = nullptr;

    } else if (biggest_block->getSize() == block_size) {
        position = biggest_block->getPosition();
        // ... |         free        | ...
        //                ||     
        //                \/    
        // ... |       occupied      | ... 
        MemoryBlock *new_occupied_block = createNewOccupiedBlock(biggest_block->getSize(),
                                                                 biggest_block->getPosition());

        connectBlocks(biggest_block->getPreviousBlock(), new_occupied_block);
        connectBlocks(new_occupied_block, biggest_block->getNextBlock());

        releaseFreeBlock(&biggest_block);

    } else if (biggest_block->getSize() > block_size) {
        position = biggest_block->getPosition();
        // ... |         free        | ...
        //                 ||     
        //                 \/    
        // ... | occupied  |  free   | ...
        MemoryBlock *left_block_to_connect = biggest_block->getPreviousBlock();
        MemoryBlock *right_block_to_connect = biggest_block->getNextBlock();

        MemoryBlock *new_occupied_block = createNewOccupiedBlock(block_size, 
                                                                 biggest_block->getPosition());
        MemoryBlock *new_free_block = createNewFreeBlock(biggest_block->getSize() - block_size, 
                                                         biggest_block->getPosition() + block_size);

        connectBlocks(left_block_to_connect, new_occupied_block);
        connectBlocks(new_occupied_block, new_free_block);
        connectBlocks(new_free_block, right_block_to_connect);

        releaseFreeBlock(&biggest_block);
    }

    ++request_counter_;
    return position;
}

void MemoryManager::deallocate(unsigned int request_number) {
    MemoryBlock *occupied = occupied_blocks_[request_number];
    if (!occupied) {
        ++request_counter_;
        return;
    }
    if (isOccupied(occupied->getPreviousBlock()) && isOccupied(occupied->getNextBlock())) {
        // ... | occupied  |  occupied | occupied | ...
        //                       ||     
        //                       \/    
        // ... | occupied  |    free   | occupied | ...
        MemoryBlock *occupied_block = occupied_blocks_[request_number];

        MemoryBlock *new_free_block = createNewFreeBlock(occupied->getSize(), 
                                                         occupied->getPosition());
        
        connectBlocks(occupied_block->getPreviousBlock(), new_free_block);
        connectBlocks(new_free_block, occupied_block->getNextBlock());
        releaseOccupiedBlock(request_number);

    } else if (!isOccupied(occupied->getPreviousBlock()) && isOccupied(occupied->getNextBlock())) {
        // ... | free |  occupied | occupied | ...
        //                  ||     
        //                  \/    
        // ... |       free       | occupied | ...
        MemoryBlock *free_left = occupied->getPreviousBlock();
        MemoryBlock *new_free_block = createNewFreeBlock(free_left->getSize() 
                                                         + occupied->getSize(), 
                                                         free_left->getPosition());

        connectBlocks(free_left->getPreviousBlock(), new_free_block);
        connectBlocks(new_free_block, occupied->getNextBlock());

        releaseFreeBlock(&free_left);
        releaseOccupiedBlock(request_number);
    } else if (isOccupied(occupied->getPreviousBlock()) && !isOccupied(occupied->getNextBlock())) {
        // ... | occupied  |  occupied | free | ...
        //                    ||     
        //                    \/    
        // ... | occupied  |       free       | ...
        MemoryBlock *free_right = occupied->getNextBlock();
        MemoryBlock *new_free_block = createNewFreeBlock(free_right->getSize() 
                                                         + occupied->getSize(), 
                                                         occupied->getPosition());

        connectBlocks(occupied->getPreviousBlock(), new_free_block);
        connectBlocks(new_free_block, occupied->getNextBlock()->getNextBlock());

        releaseFreeBlock(&free_right);
        releaseOccupiedBlock(request_number);
    } else {
        // ... | free  |  occupied | free | ...
        //                   ||     
        //                   \/    
        // ... |            free          | ...
        MemoryBlock *free_left = occupied->getPreviousBlock();
        MemoryBlock *free_right = occupied->getNextBlock();

        MemoryBlock *new_free_block = createNewFreeBlock(free_left->getSize() 
                                                        + occupied->getSize() 
                                                        + free_right->getSize(),
                                                        free_left->getPosition());

        connectBlocks(free_left->getPreviousBlock(), new_free_block); 
        connectBlocks(new_free_block, free_right->getNextBlock());
        
        releaseFreeBlock(&free_left);
        releaseFreeBlock(&free_right);        
        releaseOccupiedBlock(request_number);
    }
    ++request_counter_;
}

void MemoryManager::connectBlocks(MemoryBlock *previous, MemoryBlock *next) {
    if (previous) {
        previous->setNextBlock(next);
    }
    if (next) {
        next->setPreviousBlock(previous);
    }
}

bool MemoryManager::isOccupied(const MemoryBlock *ptr) const {
    if (ptr) {
        return ptr->getFreeHeapIndex() == MemoryBlock::OCCUPIED_BLOCK;
    } else {
        return true;
    }
}

