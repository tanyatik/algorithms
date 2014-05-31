#include <gtest/gtest.h>

#include "../thread_safe_queue.hpp"
#include "test_helper.hpp"

namespace tanyatik {

const int THREADS_COUNT = 100;
const int ELEMENTS_COUNT = 100;

void writeProcedure(ThreadSafeQueue<int> *queue, const std::vector<int> &data) {
    for (int i = 0; i < data.size(); ++i) {
        queue->push(data[i]);
    }
}

void readProcedure(ThreadSafeQueue<int> *queue, std::vector<int> *result_data) {
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
        int value = 0;
        while (!queue->tryPop(&value)) 
            ;
        result_data->push_back(value); 
    }
}

TEST(ThreadSafeQueue, multithread_push_tryPop) {
    testMultithreadContainer<ThreadSafeQueue>(writeProcedure, 
            readProcedure, 
            THREADS_COUNT, 
            ELEMENTS_COUNT);
}

} // namespace tanyatik
