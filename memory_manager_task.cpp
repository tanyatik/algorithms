#include <binary_heap.hpp>

struct MemoryBlock {
    struct KeyT {
        size_t block_size_;
        size_t block_position_;
        
        bool operator < (struct KeyT other);
    };
    size_t block_size_;
    size_t block_position_;
    MemoryBlock *p_prev_;
    MemoryBlock *p_next_;
    int free_heap_index_; // stores -1 if block is occupied

    MemoryBlock(size_t block_size_, size_t block_position);
};

MemoryBlock::MemoryBlock(size_t block_size, size_t block_position) :
    block_size_(block_size),
    block_position_(block_position),
    p_prev_(nullptr), 
    p_next_(nullptr) {}

MemoryBlock::KeyT getKey() const {
    return KeyT(block_size_, block_position_);
}

bool MemoryBlock::KeyT::operator <(struct KeyT other) {
    return ((block_size < other.block_size) || 
            (block_size == other.block_size && block_position_ < other.block_position));
}

int main() {

}
