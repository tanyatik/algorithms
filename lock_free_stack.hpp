#pragma once 

#include <atomic>

#include <gtest/gtest.h>

namespace tanyatik {

template<typename T>
class LockFreeStack {
private:
    struct Node {
        T data;
        Node *next;

        Node(const T &data) :
            data(data),
            next(nullptr)
            {}
    };

    std::atomic<int> in_pop_;
    std::atomic<int> delete_size_;
    std::atomic<Node *> head_;
    std::atomic<Node *> delete_list_;

public:
    LockFreeStack() :
        in_pop_(0),
        head_(nullptr),
        delete_list_(nullptr)
        {}

    void push(const T &data) {
        auto t = new Node(data);
        t->next = head_.load();
        while (!head_.compare_exchange_weak(t->next, t))
            ;
    }

    bool pop(T &data) {
        ++in_pop_;
        Node *t = head_.load();
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

    void clean(Node *t) {
        if(in_pop_ == 1) {
            Node *list = delete_list_.exchange(nullptr);
            if (--in_pop_ == 0) {
                delete_size_ = 0;
                doDelete(list);
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

    void doDelete(Node *node_list) {
        Node *pointer = node_list;
        while (pointer) {
            auto to_delete = pointer;
            pointer = pointer->next;
            delete to_delete;
        }
    }

    void enqueue(Node *begin, Node *end) {
        end->next = delete_list_.load();
        while (!delete_list_.compare_exchange_weak(end->next, begin))
            ;
    }

    void enqueue(Node *list) {
        Node *begin = list, *end = list;
        while (end->next != nullptr) {
            end = end->next;
        }
        enqueue(list, end);
    }

    int getDeleteQueueSize() {
        return delete_size_;
    }
};

} // namespace tanyatik
