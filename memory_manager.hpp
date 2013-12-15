#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <list>

#include "binary_heap.hpp"

class MemoryBlock {
private:
    unsigned int size_;
    unsigned int position_;

    // Stores index in heap of free blocks,
    // or OCCUPIED_BLOCK if this block is not in this heap
    int free_heap_index_;

public:
    static const int OCCUPIED_BLOCK = -1;

    MemoryBlock();
    MemoryBlock(unsigned int size, unsigned int position);

    unsigned int getSize() const;
    unsigned int getPosition() const;
    int getFreeHeapIndex() const;

    void setFreeHeapIndex(int free_heap_index);
};

struct MemoryBlockComparator {
    bool operator () (const std::list<MemoryBlock>::iterator one, 
                      const std::list<MemoryBlock>::iterator other) const {
        return ((one->getSize() < other->getSize()) || 
                (one->getSize() == other->getSize() && one->getPosition() > other->getPosition()));
    }
};

struct MemoryBlockMoveCallback {
    void operator () (const std::list<MemoryBlock>::iterator block, int index) const {
        block->setFreeHeapIndex(index);
    }
};

class MemoryManager {
    public:
        MemoryManager(unsigned memory_cells_number);

        int allocate(unsigned int block_size);
        void deallocate(unsigned int request_number);

    private:
        unsigned int memory_cells_number_;

        // Stored all blocks as they are located in the memory
        // There can be no free blocks placed side-by-side
        std::list<MemoryBlock> all_blocks_;
        // Indices in the array are numbers of queries
        // Each array element with index 'i' stores either a occupied block,
        // which was allocated as result of request number 'i',
        // or nullptr to indicate that no block was allocated on request number 'i' 
        std::vector<std::list<MemoryBlock>::iterator> allocation_results_;
        // Stores all free blocks 
        // When free block is occupied, it is deleted from the heap
        BinaryHeap<std::list<MemoryBlock>::iterator, 
                   MemoryBlockComparator, 
                   MemoryBlockMoveCallback> free_blocks_;

        // Returns true if 'block' is occupied block or if 'block' is invalid
        // (is equal to all_blocks_.end())
        bool isOccupied(const std::list<MemoryBlock>::iterator block) const;
};

MemoryBlock::MemoryBlock(unsigned int size, 
                         unsigned int position) :
    size_(size),
    position_(position),
    free_heap_index_(OCCUPIED_BLOCK) {}

unsigned int MemoryBlock::getSize() const {
    return size_;
}

unsigned int MemoryBlock::getPosition() const {
    return position_;
}

int MemoryBlock::getFreeHeapIndex() const {
    return free_heap_index_;
}

void MemoryBlock::setFreeHeapIndex(int free_heap_index) {
    free_heap_index_ = free_heap_index;
}

void swap (std::list<MemoryBlock>::iterator &one, std::list<MemoryBlock>::iterator &other) {
    int temp = one->getFreeHeapIndex();
    one->setFreeHeapIndex(other->getFreeHeapIndex());
    other->setFreeHeapIndex(temp);

    std::swap(one, other);
}


MemoryManager::MemoryManager(unsigned int memory_cells_number) :
    memory_cells_number_(memory_cells_number) {

    all_blocks_.insert(all_blocks_.begin(), MemoryBlock(memory_cells_number, 0));
    free_blocks_.insert(all_blocks_.begin());
}

int MemoryManager::allocate(unsigned int block_size) {
    int position = -1;

    std::list<MemoryBlock>::iterator biggest_block = 
        free_blocks_.getSize() > 0 ? free_blocks_.getTop() : all_blocks_.end();

    if (biggest_block == all_blocks_.end() || biggest_block->getSize() < block_size) {
        allocation_results_.push_back(all_blocks_.end());

    } else if (biggest_block->getSize() == block_size) {
        position = biggest_block->getPosition();
        // ... |         free        | ...
        //                ||     
        //                \/    
        // ... |       occupied      | ... 
        allocation_results_.push_back(biggest_block);
        free_blocks_.removeElementByIndex(biggest_block->getFreeHeapIndex());
        biggest_block->setFreeHeapIndex(MemoryBlock::OCCUPIED_BLOCK);

    } else if (biggest_block->getSize() > block_size) {
        position = biggest_block->getPosition();
        // ... |         free        | ...
        //                 ||     
        //                 \/    
        // ... | occupied  |  free   | ...

        MemoryBlock new_occupied_block = MemoryBlock(block_size,
                                                     biggest_block->getPosition());

        auto new_occupied_block_iterator = all_blocks_.insert(biggest_block, new_occupied_block);
        allocation_results_.push_back(new_occupied_block_iterator);

        MemoryBlock new_free_block = MemoryBlock(biggest_block->getSize() - block_size,
                                                 biggest_block->getPosition() + block_size);
        auto new_free_block_iterator = all_blocks_.insert(biggest_block, new_free_block);
        free_blocks_.removeElementByIndex(biggest_block->getFreeHeapIndex());
        free_blocks_.insert(new_free_block_iterator);

        all_blocks_.erase(biggest_block);
    }

    return position;
}

void MemoryManager::deallocate(unsigned int request_number) {
    
    allocation_results_.push_back(all_blocks_.end());
    std::list<MemoryBlock>::iterator occupied = allocation_results_[request_number];
    allocation_results_[request_number] = all_blocks_.end();

    std::list<MemoryBlock>::iterator left = occupied;
    --left;
    std::list<MemoryBlock>::iterator right = occupied;
    ++right;

    if (occupied == all_blocks_.end()) {
        return;
    }
    if (isOccupied(left) && isOccupied(right)) {
        // ... | occupied  |  occupied | occupied | ...
        //                       ||     
        //                       \/    
        // ... | occupied  |    free   | occupied | ...
        free_blocks_.insert(occupied);

    } else if (!isOccupied(left) && isOccupied(right)) {
        // ... | free |  occupied | occupied | ...
        //                  ||     
        //                  \/    
        // ... |       free       | occupied | ...
        MemoryBlock new_free_block(left->getSize() + occupied->getSize(), 
                                   left->getPosition());
        auto new_free_block_iterator = all_blocks_.insert(right, new_free_block);
        free_blocks_.insert(new_free_block_iterator);

        free_blocks_.removeElementByIndex(left->getFreeHeapIndex());

        all_blocks_.erase(occupied);
        all_blocks_.erase(left);


    } else if (isOccupied(left) && !isOccupied(right)) {
        // ... | occupied  |  occupied | free | ...
        //                    ||     
        //                    \/    
        // ... | occupied  |       free       | ...
         MemoryBlock new_free_block(right->getSize() + occupied->getSize(), 
                                    occupied->getPosition());
        auto new_free_block_iterator = all_blocks_.insert(right, new_free_block);
        free_blocks_.insert(new_free_block_iterator);

        free_blocks_.removeElementByIndex(right->getFreeHeapIndex());

        all_blocks_.erase(occupied);
        all_blocks_.erase(right);


    } else {
        // ... | free  |  occupied | free | ...
        //                   ||     
        //                   \/    
        // ... |            free          | ...
        MemoryBlock new_free_block(left->getSize() + 
                                   occupied->getSize() + 
                                   right->getSize(),
                                   left->getPosition());
        auto new_free_block_iterator = all_blocks_.insert(left, new_free_block);
        free_blocks_.insert(new_free_block_iterator);

        free_blocks_.removeElementByIndex(left->getFreeHeapIndex());
        free_blocks_.removeElementByIndex(right->getFreeHeapIndex());

        all_blocks_.erase(left);
        all_blocks_.erase(occupied);
        all_blocks_.erase(right);
    }
}

bool MemoryManager::isOccupied(const std::list<MemoryBlock>::iterator ptr) const {
    if (ptr != all_blocks_.end()) {
        return ptr->getFreeHeapIndex() == MemoryBlock::OCCUPIED_BLOCK;
    } else {
        return true;
    }
}

