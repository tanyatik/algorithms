#include "binary_heap.hpp"

struct MemoryBlock {
    struct KeyT {
        size_t block_size;
        size_t block_position;
        
        KeyT(size_t block_size = 0, size_t block_position = 0);
        bool operator < (struct KeyT other);
    };
    size_t size;
    size_t position;
    MemoryBlock *p_prev;
    MemoryBlock *p_next;
    int free_heap_index; // stores -1 if block is occupied

    MemoryBlock();
    MemoryBlock(size_t size, size_t position, int free_heap_index);
    MemoryBlock(size_t size, 
                size_t position, 
                int free_heap_index, 
                MemoryBlock *p_prev, 
                MemoryBlock *p_next);
    KeyT getKey() const;
    bool isOccupied() const;
};

template<>
struct TraitsSentinel<MemoryBlock::KeyT> {
    static MemoryBlock::KeyT getMinusSentinel() { return MemoryBlock::KeyT(0, 0); };
};


MemoryBlock::MemoryBlock() :
    size(0),
    position(0),
    p_prev(nullptr), 
    p_next(nullptr),
    free_heap_index(0) {}

MemoryBlock::MemoryBlock(size_t size, size_t position, int free_heap_index) :
    size(size),
    position(position),
    p_prev(nullptr), 
    p_next(nullptr),
    free_heap_index(free_heap_index) {}

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

MemoryBlock::KeyT::KeyT(size_t block_size, size_t block_position) :
    block_size(block_size),
    block_position(block_position) {}

bool MemoryBlock::KeyT::operator <(struct KeyT other) {
    return ((block_size < other.block_size) || 
            (block_size == other.block_size && block_position < other.block_position));
}

class MemoryManager {
    public:
        MemoryManager(int memory_cells_number, int max_query_number);

        int allocate(int block_size);
        void deallocate(int query_number);

    private:
        int memory_cells_number_;
        int query_number_;
        int query_counter_;

        std::vector<MemoryBlock> occupied_blocks_;
        typedef BinaryHeap<MemoryBlock::KeyT, MemoryBlock> TFreeBlocksHeap; 
        TFreeBlocksHeap free_blocks_;

        MemoryBlock& updateFreeBlockLinks(TFreeBlocksHeap::TIndex free_idx);
};

MemoryManager::MemoryManager(int memory_cells_number, int query_number) :
    memory_cells_number_(memory_cells_number),
    query_number_(query_number),
    query_counter_(0),
    occupied_blocks_(query_number),
    free_blocks_(query_number) {
    MemoryBlock free_block(memory_cells_number, 0, 0, nullptr, nullptr);
    free_blocks_.insert(free_block.getKey(), free_block);
}

int MemoryManager::allocate(int block_size) {
    if (free_blocks_.getSize() < 1) {
        return -1;
    }
    int biggest_block_size = free_blocks_.getMax().size;
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

        MemoryBlock new_occ_block = MemoryBlock(block_size, 
                                                biggest_block.position, 
                                                -1, 
                                                biggest_block.p_prev, 
                                                biggest_block.p_next);

        occupied_blocks_[query_counter_++] = new_occ_block;
    } else if (biggest_block_size > block_size) {
        // ... |         free        | ...
        //                 ||     
        //                 \/    
        // ... | occ  |      free    | ...
        // splitting free block into occupied block and free block
        MemoryBlock biggest_block = free_blocks_.extractMax();

        // create occupied block
        MemoryBlock& new_occ_block = occupied_blocks_[query_counter_++] = 
            MemoryBlock(block_size, biggest_block.position, -1, biggest_block.p_prev, nullptr);
        // create free block
        MemoryBlock tmp_free_block = MemoryBlock(biggest_block.size - block_size, 
                                                 biggest_block.position + block_size, 
                                                 -1, 
                                                 &new_occ_block, 
                                                 biggest_block.p_next); 
        auto free_idx = free_blocks_.insert(tmp_free_block.getKey(), tmp_free_block);
        MemoryBlock& new_free_block = updateFreeBlockLinks(free_idx);

        new_occ_block.p_next = &new_free_block;
    }
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
        auto free_idx = free_blocks_.insert(occ.getKey(), occ);
        MemoryBlock& new_free_block = updateFreeBlockLinks(free_idx);
    } else if (!occ.p_prev->isOccupied() && occ.p_next->isOccupied()) {
        // ... | free |  occ | occ | ...
        //               ||     
        //               \/    
        // ... |   free      | occ | ...
        
        // updating free block
        MemoryBlock& free_left = *occ.p_prev;

        MemoryBlock tmp(free_left.size + occ.size, 
                        free_left.position,
                        -1,
                        free_left.p_prev,
                        occ.p_next);
        auto free_idx = free_blocks_.replace(free_left.free_heap_index, tmp.getKey(), tmp);
        MemoryBlock& new_free_block = updateFreeBlockLinks(free_idx);
    } else if (occ.p_prev->isOccupied() && !occ.p_next->isOccupied()) {
        // ... | occ  |  occ | free | ...
        //               ||     
        //               \/    
        // ... | occ  |    free     | ...
        
        // updating free block
        MemoryBlock& free_right = *occ.p_next;

        MemoryBlock tmp(free_right.size + occ.size, 
                        occ.position,
                        -1,
                        occ.p_prev,
                        free_right.p_next);
        auto free_idx = free_blocks_.replace(free_right.free_heap_index, tmp.getKey(), tmp);
        MemoryBlock& new_free_block = updateFreeBlockLinks(free_idx);
    }
    else {
        // ... | free  |  occ | free | ...
        //                 ||     
        //                 \/    
        // ... |         free        | ...

        // updating previous free block
        MemoryBlock& free_left = *occ.p_prev;
        MemoryBlock& free_right = *occ.p_next;

        MemoryBlock tmp(free_left.size + occ.size + free_right.size, 
                        free_left.position, 
                        -1,
                        free_left.p_prev, 
                        free_right.p_prev);
        auto new_idx = free_blocks_.replace(free_left.free_heap_index, tmp.getKey(), tmp);
        MemoryBlock& new_free_block = updateFreeBlockLinks(new_idx);
        free_blocks_.remove(free_right.free_heap_index);
    }
}

MemoryBlock& MemoryManager::updateFreeBlockLinks(TFreeBlocksHeap::TIndex free_idx) {
    MemoryBlock& new_free_block = free_blocks_[free_idx];
    new_free_block.free_heap_index = free_idx;
    if (new_free_block.p_prev) {
        new_free_block.p_prev->p_next = &new_free_block;
    }
    if (new_free_block.p_next) {
        new_free_block.p_next->p_prev = &new_free_block;
    }
}

int main() {
    MemoryManager manager(1000, 4);
    manager.allocate(500);
    manager.allocate(250);
    manager.allocate(125);
    //manager.deallocate(2);
}
