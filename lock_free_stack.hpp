#include <atomic>

#include <gtest/gtest.h>

template<typename T>
class lock_free_stack_t {
private:
    struct node_t {
        T data;
        node_t *next;

        node_t(const T &data) :
            data(data),
            next(nullptr)
            {}
    };

    std::atomic<int> in_pop_;
    std::atomic<int> delete_size_;
    std::atomic<node_t *> head_;
    std::atomic<node_t *> delete_list_;

public:
    lock_free_stack_t() :
        in_pop_(0),
        head_(nullptr),
        delete_list_(nullptr)
        {}

    void push(const T &data) {
        auto t = new node_t(data);
        t->next = head_.load();
        while (!head_.compare_exchange_weak(t->next, t))
            ;
    }

    bool pop(T &data) {
        ++in_pop_;
        node_t *t = head_.load();
        while(t && !head_.compare_exchange_weak(t, t->next))
            ;
        if (!t) {
            return false;
        } else {
            data = t->data;
            clean(t);
        }
        return true;
    }

    void clean(node_t *t) {
        if(in_pop_ == 1) {
            node_t *list = delete_list_.exchange(nullptr);
            if (--in_pop_ == 0) {
                delete_size_ = 0;
                do_delete(list);
            } else if (list) {
                enqueue(list);
            }
            delete t;
        } else {
            enqueue(t, t);
            delete_size_++;
            --in_pop_;
        }
    }

    void do_delete(node_t *node_list) {
        node_t *pointer = node_list;
        while (pointer) {
            auto to_delete = pointer;
            pointer = pointer->next;
            delete to_delete;
        }
    }

    void enqueue(node_t *begin, node_t *end) {
        end->next = delete_list_.load();
        while (!delete_list_.compare_exchange_weak(end->next, begin))
            ;
    }

    void enqueue(node_t *list) {
        node_t *begin = list, *end = list;
        while (end->next != nullptr) {
            end = end->next;
        }
        enqueue(list, end);
    }

    int get_delete_queue_size() {
        return delete_size_;
    }
};
