#include <atomic>

template<typename T>
class LockFreeQueue {
private:
    struct ListNode {
        T data;
        std::atomic<ListNode *> next;

        ListNode(const T &data) :
            data(data),
            next(nullptr)
            {}
    };

    struct RootNode;

    typedef std::atomic<ListNode *> ListNodePtr;
    typedef std::atomic<RootNode *> RootNodePtr;

    struct RootNode {
        ListNodePtr push_queue;
        ListNodePtr pop_queue;
        ListNodePtr to_delete;
        RootNode *next_former_root;
    };

    RootNodePtr actual_root;
    std::atomic<int> free_mem_counter_;

    RootNodePtr former_root_;

public: 
    LockFreeQueue() :
        actual_root(new RootNode()),
        free_mem_counter_(0),
        former_root_(nullptr)
        {}

    ~LockFreeQueue() {
        RootNode *current = actual_root.load();
        incActiveThreads();
        decActiveThreads();
        deleteList(current->push_queue);
        deleteList(current->pop_queue);
        delete current;
    }

    void enqueue(const T &data) {
        // create new element
        ListNode *node(new ListNode(data));

        // make a copy
        RootNode *new_root(new RootNode());

        incActiveThreads();
        
        while (true) {
            // try update actual_root with new_root,
            // which is copied actual_root with new_node
            RootNode *current = actual_root.load();

            new_root->push_queue = node;
            node->next = current->push_queue.load();

            new_root->pop_queue = current->pop_queue.load();

            // expect new_root in actual_root, if yes, put current into actual_root
            if (actual_root.compare_exchange_weak(current, new_root)) {
                decActiveThreads();
                pushDeleteRoot(current);
                break;
            }  
        }
    }

    void pushDeleteRoot(RootNode *current) {
        while(true) {
            current->next_former_root = former_root_.load();
    
            if (former_root_.compare_exchange_strong(current->next_former_root, current)) {
                break;    
            } 
        }
    }

    void tryDeleteExpiredRoots() {
        RootNode *curent_former = former_root_.load();

        if (!curent_former) {
            return;
        }


        // if the only thread tries to work with queue
        if (free_mem_counter_ == 1) {
            // expect nullptr, if yes, put curent_former into former_root_
            
            RootNode *desired = nullptr;
            if (former_root_.compare_exchange_strong(curent_former, desired)) {
                while (curent_former) {
                    auto tmp = curent_former->next_former_root;
                    deleteList(curent_former->to_delete);
                    delete curent_former;
                    curent_former = tmp;
                }
            }
        }
    }

    void deleteList(ListNode *node_list) {
        ListNode *pointer = node_list;
        while (pointer) {
            auto deleted = pointer;
            pointer = pointer->next;
            delete deleted;
        }
    }

    void incActiveThreads() {
        ++free_mem_counter_;
    }

    void decActiveThreads() {
        tryDeleteExpiredRoots();
        --free_mem_counter_;
    }
};
