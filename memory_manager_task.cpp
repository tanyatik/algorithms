#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>

struct MemoryBlock {
    struct KeyT {
        unsigned int block_size;
        unsigned int block_position;
        
        KeyT(unsigned int block_size = 0, unsigned int block_position = 0);
        bool operator < (struct KeyT other);
        void debugPrint() const;
    };
    unsigned int size;
    unsigned int position;
    MemoryBlock *p_prev;
    MemoryBlock *p_next;
    int free_heap_index; // stores -1 if block is occupied

    MemoryBlock();
    MemoryBlock(unsigned int size, unsigned int position);
    MemoryBlock(unsigned int size, 
                unsigned int position, 
                long long free_heap_index, 
                MemoryBlock *p_prev, 
                MemoryBlock *p_next);
    KeyT getKey() const;
    void debugPrint() const;
    void swap(MemoryBlock &other);
};

MemoryBlock::MemoryBlock() :
    size(0),
    position(0),
    p_prev(nullptr), 
    p_next(nullptr),
    free_heap_index(0) {}

MemoryBlock::MemoryBlock(unsigned int size, unsigned int position) :
    size(size),
    position(position),
    p_prev(nullptr), 
    p_next(nullptr),
    free_heap_index(-1) {}

MemoryBlock::KeyT MemoryBlock::getKey() const {
    return KeyT(size, position);
}

void MemoryBlock::debugPrint() const {
    std::cout << "[p" << position << ":s" << size << ':' 
        << free_heap_index  << "]";
}

MemoryBlock::KeyT::KeyT(unsigned int block_size, unsigned int block_position) :
    block_size(block_size),
    block_position(block_position) {}

bool MemoryBlock::KeyT::operator <(struct KeyT other) {
    return ((block_size < other.block_size) || 
            (block_size == other.block_size && block_position > other.block_position));
}

void MemoryBlock::KeyT::debugPrint() const {
    std::cout << block_size << ':' << block_position;
}

// Class representing binary heap with user specified key type and value type
class MemoryBinaryHeap {
    public:
        explicit MemoryBinaryHeap(std::size_t max_size);
        long long extractMax();
        const MemoryBlock *getMax() const;
        long long insert(MemoryBlock::KeyT key);
        MemoryBlock *&operator[] (long long index);
        MemoryBlock *const &operator[] (long long index) const;
        long long replace(long long prev_idx, MemoryBlock::KeyT new_key);
        long long remove(long long idx);
        void debugPrint() const;
        unsigned int getSize() const;

    private:
        void heapifyDown(long long idx);
        long long heapifyUp(long long idx);
        long long increaseKey(MemoryBlock::KeyT new_key, long long idx);
        void swapElements(long long one, long long other);
        long long getParentIndex(long long idx) const;
        long long getLeftIndex(long long idx) const;
        long long getRightIndex(long long idx) const;
        void deleteElement(long long idx);

        unsigned int heap_size_;
        unsigned int max_heap_size_;
        std::vector<MemoryBlock::KeyT> keys_;
        std::vector<MemoryBlock*> values_;
};

MemoryBinaryHeap::MemoryBinaryHeap(std::size_t max_size) : 
    heap_size_(0),
    max_heap_size_(max_size),
    keys_(max_heap_size_),
    values_(max_heap_size_) {}

const MemoryBlock *MemoryBinaryHeap::getMax() const {
    return values_[0];
}

long long MemoryBinaryHeap::getParentIndex(long long idx) const {
    if (idx <= 0) return -1;
    return (idx - 1) >> 1;
}

long long MemoryBinaryHeap::getLeftIndex(long long idx) const {
    return (idx << 1) + 1;
}

long long MemoryBinaryHeap::getRightIndex(long long idx) const {
    return (idx << 1) + 2;
}

void MemoryBinaryHeap::swapElements(long long one, long long other) {
    using std::swap;
    swap(keys_[one], keys_[other]);
    swap(values_[one], values_[other]);
    if (values_[one]) {
        values_[one]->free_heap_index = one;
    }
    if (values_[other]) {
        values_[other]->free_heap_index = other;
    }
}

unsigned int MemoryBinaryHeap::getSize() const {
    return heap_size_;
}

MemoryBlock *&MemoryBinaryHeap::operator[] (long long idx) {
    // just as an experiment
    return const_cast<MemoryBlock *&>(static_cast<const MemoryBinaryHeap&>(*this)[idx]);
}

MemoryBlock * const &MemoryBinaryHeap::operator[] (long long idx) const {
    return values_[idx];
}

void MemoryBinaryHeap::heapifyDown(long long idx) {
    long long left_idx = getLeftIndex(idx);
    long long right_idx = getRightIndex(idx);
    long long max_idx = idx;

    if (left_idx < heap_size_ && keys_[max_idx] < keys_[left_idx]) {
        max_idx = left_idx;
    }
    if (right_idx < heap_size_ && keys_[max_idx] < keys_[right_idx]) {
        max_idx = right_idx;
    }
    if (max_idx != idx) {
        swapElements(idx, max_idx);
        heapifyDown(max_idx);
    }
}

long long MemoryBinaryHeap::insert(MemoryBlock::KeyT key) {
    keys_[heap_size_] = MemoryBlock::KeyT(0, 0);
    long long new_idx = increaseKey(key, heap_size_);

    ++heap_size_;
    return new_idx;
}

long long MemoryBinaryHeap::replace(long long prev_idx, 
                                       MemoryBlock::KeyT new_key) {
    long long idx = increaseKey(new_key, prev_idx);

    return idx;
}

long long MemoryBinaryHeap::extractMax() {
    deleteElement(0);
    heapifyDown(0);
    return heap_size_;
}

long long MemoryBinaryHeap::remove(long long idx) {
    deleteElement(idx);

    idx = heapifyUp(idx);
    heapifyDown(idx);
    return heap_size_;
}

long long MemoryBinaryHeap::increaseKey(MemoryBlock::KeyT new_key, long long idx) {
    keys_[idx] = new_key;
    idx = heapifyUp(idx);
    return idx;
}

void MemoryBinaryHeap::deleteElement(long long idx) {
    swapElements(idx, heap_size_ - 1);
    keys_[heap_size_ - 1] = MemoryBlock::KeyT();
    --heap_size_;
}

void MemoryBinaryHeap::debugPrint() const {
    for (auto k_it = keys_.begin(); k_it != keys_.end(); ++k_it) {
        k_it->debugPrint();
        std::cout << '\t';
    }
    std::cout << "\n";
}

long long MemoryBinaryHeap::heapifyUp(long long idx) {
    long long parent_idx = getParentIndex(idx);

    while (idx > 0 && keys_[parent_idx] < keys_[idx]) {
        swapElements(idx, parent_idx);
        idx = parent_idx;
        parent_idx = getParentIndex(idx);
    }
    return idx;
}


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
         
        MemoryBinaryHeap free_blocks_;
        MemoryBlock *head_;

        void connectBlocks(MemoryBlock *one, MemoryBlock *other);
        bool isOccupied(const MemoryBlock *block) const;
};

MemoryManager::MemoryManager(unsigned int memory_cells_number, unsigned int query_number) :
    memory_cells_number_(memory_cells_number),
    query_number_(query_number),
    query_counter_(0),
    occupied_blocks_(query_number),
    free_blocks_(query_number),
    head_(new MemoryBlock(0, 0)) {
    MemoryBlock *free_block = new MemoryBlock(memory_cells_number, 0);
    auto idx = free_blocks_.insert(free_block->getKey());
    free_blocks_[idx] = free_block;
    free_block->free_heap_index = idx;
    head_->p_next = free_block;
    free_block->p_prev = head_;
}

MemoryManager::~MemoryManager() {
    for (unsigned int idx = 0; idx < query_number_; ++idx) {
        if (occupied_blocks_[idx] != nullptr) {
            delete occupied_blocks_[idx];
            occupied_blocks_[idx] = nullptr;
        }
        if (free_blocks_[idx] != nullptr) {
            delete free_blocks_[idx];
            free_blocks_[idx] = nullptr;
        }
    }
    delete head_;
}


long long MemoryManager::allocate(unsigned int block_size) {
    long long position = -1;
    const MemoryBlock *biggest_block = free_blocks_.getMax();
    if (!biggest_block || biggest_block->size < block_size) {
        occupied_blocks_[query_counter_] = nullptr;
        position = -1;
    } else if (biggest_block->size == block_size) {
        position = biggest_block->position + 1;
        // ... |         free        | ...
        //                ||     
        //                \/    
        // ... |         occ         | ...     occ for occupied here

        // whole free block turns to occupied block
        long long idx = free_blocks_.extractMax();
        MemoryBlock *biggest_block = free_blocks_[idx];
        biggest_block->free_heap_index = -1;
        occupied_blocks_[query_counter_] = biggest_block;
        free_blocks_[idx] = nullptr;
    } else if (biggest_block->size > block_size) {
        position = biggest_block->position + 1;
        // ... |         free        | ...
        //                 ||     
        //                 \/    
        // ... | occ  |      free    | ...
        // splitting free block into occupied block and free block
        auto idx = free_blocks_.extractMax();
        MemoryBlock *biggest_block = free_blocks_[idx];
        free_blocks_[idx] = nullptr;

        // create occupied block
        occupied_blocks_[query_counter_] = new MemoryBlock(block_size, biggest_block->position);
        MemoryBlock *new_occ_block = occupied_blocks_[query_counter_];
        // create free block
        MemoryBlock *new_free_block = new MemoryBlock(biggest_block->size - block_size, 
                                                 biggest_block->position + block_size);
        auto free_idx = free_blocks_.insert(new_free_block->getKey());
        free_blocks_[free_idx] = new_free_block;
        new_free_block->free_heap_index = free_idx;

        connectBlocks(biggest_block->p_prev, new_occ_block);
        connectBlocks(new_occ_block, new_free_block);
        connectBlocks(new_free_block, biggest_block->p_next);

        delete biggest_block;
    }
    ++query_counter_;
    return position;
}

void MemoryManager::deallocate(unsigned int query_number) {
    --query_number;
    MemoryBlock *occ = occupied_blocks_[query_number]; // occ -- occupied block to free
    if (!occ) {
        ++query_counter_;
        return;
    }
    if (isOccupied(occ->p_prev) && isOccupied(occ->p_next)) {
        // ... | occ  |  occ | occ | ...
        //               ||     
        //               \/    
        // ... | occ  | free | occ | ...

        // creating free block
        auto free_idx = free_blocks_.insert(occ->getKey());
        free_blocks_[free_idx] = occ;
        occ->free_heap_index = free_idx;
        occupied_blocks_[query_number] = nullptr;
    } else if (!isOccupied(occ->p_prev) && isOccupied(occ->p_next)) {
        // ... | free |  occ | occ | ...
        //               ||     
        //               \/    
        // ... |   free      | occ | ...
        
        // updating free block
        MemoryBlock *free_left = occ->p_prev;

        MemoryBlock *new_free_block = new MemoryBlock(free_left->size + occ->size, 
                                                      free_left->position);
        connectBlocks(free_left->p_prev, new_free_block);
        connectBlocks(new_free_block, occ->p_next);

        long long free_idx = free_blocks_.replace
            (free_left->free_heap_index, new_free_block->getKey());
        delete free_blocks_[free_idx];
        free_blocks_[free_idx] = new_free_block;
        new_free_block->free_heap_index = free_idx;

        delete occ;
        occupied_blocks_[query_number] = nullptr;
    } else if (isOccupied(occ->p_prev) && !isOccupied(occ->p_next)) {
        // ... | occ  |  occ | free | ...
        //               ||     
        //               \/    
        // ... | occ  |    free     | ...
        
        // updating free block
        MemoryBlock *free_right = occ->p_next;

        MemoryBlock *new_free_block = new MemoryBlock(free_right->size + occ->size, occ->position);
        connectBlocks(occ->p_prev, new_free_block);
        connectBlocks(new_free_block, occ->p_next->p_next);

        auto free_idx = free_blocks_.replace(free_right->free_heap_index, 
                                             new_free_block->getKey());
        delete free_blocks_[free_idx];
        free_blocks_[free_idx] = new_free_block;
        new_free_block->free_heap_index = free_idx;


        delete occ;
        occupied_blocks_[query_number] = nullptr;
    } else {
        // ... | free  |  occ | free | ...
        //                 ||     
        //                 \/    
        // ... |         free        | ...

        // updating previous free block
        MemoryBlock *free_left = occ->p_prev;
        MemoryBlock *free_right = occ->p_next;

        MemoryBlock *new_free_block = new MemoryBlock(free_left->size 
                                                       + occ->size 
                                                       + free_right->size,
                                                      free_left->position);
        connectBlocks(free_left->p_prev, new_free_block); //
        connectBlocks(new_free_block, free_right->p_next);

        auto new_idx = free_blocks_.replace(free_left->free_heap_index, new_free_block->getKey());
        MemoryBlock *old_free_left = free_blocks_[new_idx];
        assert(old_free_left == free_left);
        free_blocks_[new_idx] = new_free_block;
        new_free_block->free_heap_index = new_idx;

        auto delete_idx = free_blocks_.remove(occ->p_next->free_heap_index);
        delete free_blocks_[delete_idx];
        free_blocks_[delete_idx] = nullptr;

        delete occ;
        occupied_blocks_[query_number] = nullptr;

        delete old_free_left;
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
    MemoryBlock *ptr = head_->p_next;
    while (ptr) {
        ptr->debugPrint();
        std::cout << ' ';
        ptr = ptr->p_next;
    }
    std::cout << std::endl;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    unsigned int mem_cells, query_count;
    long long query;
    std::cin >> mem_cells >> query_count;
    MemoryManager manager(mem_cells, query_count);
    for (unsigned int query_idx = 0; query_idx < query_count; ++query_idx) {
        std::cin >> query;
        if (query > 0) {
            std::cout << manager.allocate((unsigned int) query) << std::endl;
        } else {
            manager.deallocate(-query);
        }
    }
    return 0;
}
