#include "binary_heap.hpp"

struct MemoryBlock {
    struct KeyT {
        size_t block_size;
        size_t block_position;
        
        KeyT(size_t block_size = 0, size_t block_position = 0);
        bool operator < (struct KeyT other);
        void debugPrint() const;
    };
    size_t size;
    size_t position;
    MemoryBlock *p_prev;
    MemoryBlock *p_next;
    int free_heap_index; // stores -1 if block is occupied

    MemoryBlock();
    MemoryBlock(size_t size, size_t position);
    MemoryBlock(size_t size, 
                size_t position, 
                int free_heap_index, 
                MemoryBlock *p_prev, 
                MemoryBlock *p_next);
    KeyT getKey() const;
    bool isOccupied() const;
    void debugPrint() const;
    void swap(MemoryBlock &other);
};

MemoryBlock::MemoryBlock() :
    size(0),
    position(0),
    p_prev(nullptr), 
    p_next(nullptr),
    free_heap_index(0) {}

MemoryBlock::MemoryBlock(size_t size, size_t position) :
    size(size),
    position(position),
    p_prev(nullptr), 
    p_next(nullptr),
    free_heap_index(-1) {}

MemoryBlock::MemoryBlock(size_t size, 
                         size_t position, 
                         int free_heap_index, 
                         MemoryBlock *p_prev, 
                         MemoryBlock *p_next) :
    size(size),
    position(position),
    p_prev(p_prev), 
    p_next(p_next),
    free_heap_index(free_heap_index) {}

MemoryBlock::KeyT MemoryBlock::getKey() const {
    return KeyT(size, position);
}

bool MemoryBlock::isOccupied() const {
    return free_heap_index == -1;
}

void MemoryBlock::swap(MemoryBlock &other) {
    MemoryBlock tmp = *this;
    *this = other;
    other = tmp;

    if (p_prev) {
        p_prev->p_next = this;
    }
    if (p_next) {
        p_next->p_prev = this;
    }

    if (other.p_prev) {
        other.p_prev->p_next = &other;
    }
    if (other.p_next) {
        other.p_next->p_prev = &other;
    }
}

void swap(MemoryBlock &one, MemoryBlock &other) {
    one.swap(other);
}

void MemoryBlock::debugPrint() const {
    std::cout << "[" << position << ":" << size << ':' << (free_heap_index == -1 ? 'O' : 'F') << "]";
}

MemoryBlock::KeyT::KeyT(size_t block_size, size_t block_position) :
    block_size(block_size),
    block_position(block_position) {}

bool MemoryBlock::KeyT::operator <(struct KeyT other) {
    return ((block_size < other.block_size) || 
            (block_size == other.block_size && block_position < other.block_position));
}

void MemoryBlock::KeyT::debugPrint() const {
    std::cout << block_size << ':' << block_position;
}

template<>
struct TraitsSentinel<MemoryBlock::KeyT> {
    static MemoryBlock::KeyT getMinusSentinel() { return MemoryBlock::KeyT(0, 0); };
};

class MemoryManager {
    public:
        MemoryManager(int memory_cells_number, int max_query_number);

        int allocate(int block_size);
        void deallocate(int query_number);
        void debugPrint() const;
    private:
        int memory_cells_number_;
        int query_number_;
        int query_counter_;
        // debug

        std::vector<MemoryBlock> occupied_blocks_;
        typedef BinaryHeap<MemoryBlock::KeyT, MemoryBlock> TFreeBlocksHeap; 
        TFreeBlocksHeap free_blocks_;
        MemoryBlock head_;

        MemoryBlock& getFreeBlockLinks(TFreeBlocksHeap::TIndex free_idx);
        void connectBlocks(MemoryBlock *one, MemoryBlock *other);
};

MemoryManager::MemoryManager(int memory_cells_number, int query_number) :
    memory_cells_number_(memory_cells_number),
    query_number_(query_number),
    query_counter_(0),
    occupied_blocks_(query_number),
    free_blocks_(query_number),
    head_(0, 0) {
    MemoryBlock free_block(memory_cells_number, 0);
    auto idx = free_blocks_.insert(free_block.getKey(), free_block);
    connectBlocks(&head_, &free_blocks_[idx]);
}

int MemoryManager::allocate(int block_size) {
    if (free_blocks_.getSize() < 1) {
        return -1;
    }
    int biggest_block_size = free_blocks_.getMax().size;
    int position = free_blocks_.getMax().position;
    if (biggest_block_size < block_size) {
        // no place for such a big block
        return -1;
    } else if (biggest_block_size == block_size) {
        // ... |         free        | ...
        //                ||     
        //                \/    
        // ... |         occ         | ...     occ for occupied here

        // whole free block turns to occupied block
        MemoryBlock biggest_block = free_blocks_.extractMax();

        occupied_blocks_[query_counter_] = MemoryBlock(block_size, biggest_block.position);
        MemoryBlock& new_occ_block = occupied_blocks_[query_counter_];

        connectBlocks(biggest_block.p_prev, &new_occ_block);
        connectBlocks(&new_occ_block, biggest_block.p_next);
    } else if (biggest_block_size > block_size) {
        // ... |         free        | ...
        //                 ||     
        //                 \/    
        // ... | occ  |      free    | ...
        // splitting free block into occupied block and free block
        MemoryBlock biggest_block = free_blocks_.extractMax();

        // create occupied block
        occupied_blocks_[query_counter_] = MemoryBlock(block_size, biggest_block.position);
        MemoryBlock& new_occ_block = occupied_blocks_[query_counter_];
        // create free block
        MemoryBlock tmp_free_block = MemoryBlock(biggest_block.size - block_size, 
                                                 biggest_block.position + block_size);
        auto free_idx = free_blocks_.insert(tmp_free_block.getKey(), tmp_free_block);
        MemoryBlock& new_free_block = getFreeBlockLinks(free_idx);

        connectBlocks(biggest_block.p_prev, &new_occ_block);
        connectBlocks(&new_occ_block, &new_free_block);
        connectBlocks(&new_free_block, biggest_block.p_next);
    }
    ++query_counter_;
    return position;
}

void MemoryManager::deallocate(int query_number) {
    --query_number;
    if(query_number >= query_counter_ || query_number < 0)
        return;
    MemoryBlock& occ = occupied_blocks_[query_number]; // occ -- occupied block to free
    assert(occ.free_heap_index == -1); // not free

    if(occ.p_prev->isOccupied() && occ.p_next->isOccupied()) {
        // ... | occ  |  occ | occ | ...
        //               ||     
        //               \/    
        // ... | occ  | free | occ | ...

        // creating free block
        MemoryBlock tmp(occ.size, occ.position);
        auto free_idx = free_blocks_.insert(tmp.getKey(), tmp);
        MemoryBlock& new_free_block = getFreeBlockLinks(free_idx);
        connectBlocks(occ.p_prev, &new_free_block);
        connectBlocks(&new_free_block, occ.p_next);
    } else if (!occ.p_prev->isOccupied() && occ.p_next->isOccupied()) {
        // ... | free |  occ | occ | ...
        //               ||     
        //               \/    
        // ... |   free      | occ | ...
        
        // updating free block
        MemoryBlock& free_left = *occ.p_prev;

        MemoryBlock tmp(free_left.size + occ.size, free_left.position);
        auto free_idx = free_blocks_.replace(free_left.free_heap_index, tmp.getKey(), tmp);
        MemoryBlock& new_free_block = getFreeBlockLinks(free_idx);

        connectBlocks(free_left.p_next, &new_free_block);
        connectBlocks(&new_free_block, occ.p_next);
    } else if (occ.p_prev->isOccupied() && !occ.p_next->isOccupied()) {
        // ... | occ  |  occ | free | ...
        //               ||     
        //               \/    
        // ... | occ  |    free     | ...
        
        // updating free block
        MemoryBlock& free_right = *occ.p_next;

        MemoryBlock tmp(free_right.size + occ.size, occ.position);
        auto free_idx = free_blocks_.replace(free_right.free_heap_index, tmp.getKey(), tmp);
        MemoryBlock& new_free_block = getFreeBlockLinks(free_idx);

        connectBlocks(occ.p_prev, &new_free_block);
        connectBlocks(&new_free_block, free_right.p_next);
    }
    else {
        // ... | free  |  occ | free | ...
        //                 ||     
        //                 \/    
        // ... |         free        | ...

        // updating previous free block
        MemoryBlock& free_left = *occ.p_prev;
        MemoryBlock& free_right = *occ.p_next;

        MemoryBlock tmp(free_left.size + occ.size + free_right.size, free_left.position);
        auto new_idx = free_blocks_.replace(free_left.free_heap_index, tmp.getKey(), tmp);
        MemoryBlock& new_free_block = getFreeBlockLinks(new_idx);

        connectBlocks(free_left.p_prev, &new_free_block);
        connectBlocks(&new_free_block, free_right.p_next);
        free_blocks_.remove(free_right.free_heap_index);
    }
}

MemoryBlock& MemoryManager::getFreeBlockLinks(TFreeBlocksHeap::TIndex free_idx) {
    MemoryBlock& new_free_block = free_blocks_[free_idx];
    new_free_block.free_heap_index = free_idx;
    return new_free_block;
}

void MemoryManager::connectBlocks(MemoryBlock *one, MemoryBlock *other) {
    if(one) {
        one->p_next = other;
    }
    if(other) {
        other->p_prev = one;
    }
}

void MemoryManager::debugPrint() const {
    MemoryBlock *ptr = head_.p_next;
    while(ptr) {
        ptr->debugPrint();
        std::cout << ' ';
        ptr = ptr->p_next;
    }
    std::cout << std::endl;
}

int main() {
    MemoryManager manager(1000, 4);
    manager.debugPrint();
    manager.allocate(500);
    manager.debugPrint();
    manager.allocate(250);
    manager.debugPrint();
    manager.allocate(125);
    manager.debugPrint();
    manager.deallocate(2);
    manager.debugPrint();
}
