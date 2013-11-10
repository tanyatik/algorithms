#include <binary_heap.hpp>

struct MemoryBlock {
    struct KeyT {
        size_t block_size_;
        size_t block_position_;
        
        bool operator < (struct KeyT other);
    };
    size_t size;
    size_t position;
    MemoryBlock *p_prev;
    MemoryBlock *p_next;
    int free_heap_index; // stores -1 if block is occupied

    MemoryBlock(size_t size_, size_t position);
    KeyT getKey() const;
};

MemoryBlock::MemoryBlock(size_t size, size_t position) :
    size(size),
    position(position),
    p_prev(nullptr), 
    p_next(nullptr),
    free_heap_index(-1) {}

MemoryBlock::KeyT getKey() const {
    return KeyT(size, position);
}

bool MemoryBlock::KeyT::operator <(struct KeyT other) {
    return ((size < other.size) || 
            (size == other.size && position_ < other.position));
}

class MemoryManager {
    MemoryManager(int memory_cells_number, int max_query_number);

    int allocate(int block_size);
    void deallocate(int query_number);

    private:
        int memory_cells_number_;
        int query_number_;

        std::vector<MemoryBlock> occupied_blocks_;
        BinaryHeap<MemoryBlock::KeyT, MemoryBlock> free_blocks_;
};

MemoryManager::MemoryManager(int memory_cells_number, int query_number) :
    memory_cells_number_(memory_cells_number),
    query_number_(query_number),
    occupied_blocks_(query_number),
    free_blocks_(query_number)
    {
    MemoryBlock free_block(memory_cells_number, 0);
    free_blocks_.insert(free_block.getKey(), free_block);
}

int MemoryBlock::allocate(int block_size) {
    if (free_blocks_.getSize() < 1) {
        return -1;
    }
    if (free_blocks_.getMax().size < block_size) {
        // no place for such a big block
        return -1;
    } else if (biggest_block.size == block_size) {
        // whole free block turns to occupied block
        MemoryBlock biggest_block = free_blocks_.extractMax();

        MemoryBlock new_occ_block = MemoryBlock(block_size, biggest_block.position);
        new_occ_block.p_prev = biggest_block.p_prev;
        new_occ_block.p_next = biggest_block.p_next;

        occupied_blocks[query_counter++] = new_occ_block;
    } else if (biggest_block.size > block_size) {
        // splitting free block into occupied block and free block
        MemoryBlock biggest_block = free_blocks_.extractMax();

        MemoryBlock& new_occ_block = occupied_blocks[query_counter++] = 
            MemoryBlock(block_size, biggest_block.position);
        MemoryBlock tmp_free_block = MemoryBlock(biggest_block.size - block_size, 
        MemoryBlock& new_free_block = free_blocks_.insert(tmp_free_block.getKey(), tmp_free_block);
        
        new_occ_block.p_prev = biggest_block.p_prev;
        new_occ_block.p_next = &new_free_block;
        new_free_block.p_prev = &new_occ_block;
        new_free_block.p_next = biggest_block.p_next;
    }
}

MemoryBlock::deallocate(int query_number) {
}

int main() {

}
