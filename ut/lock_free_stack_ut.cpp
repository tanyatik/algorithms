#include <thread>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

#include <gtest/gtest.h>
#include "test_helper.hpp"

#include "../lock_free_stack.hpp"

namespace tanyatik {

TEST(LockFreeStack, push_pop) {
    LockFreeStack<int> stack;
    int value;

    stack.push(3);
    stack.pop(value); 

    EXPECT_EQ(3, value);
}

TEST(LockFreeStack, push_push_pop_pop) {
    LockFreeStack<int> stack;
    int value1, value2;

    stack.push(3);
    stack.push(4);
    stack.pop(value1); 
    stack.pop(value2); 

    EXPECT_EQ(4, value1);
    EXPECT_EQ(3, value2);
}

const int THREADS_COUNT = 100;
const int ELEMENTS_COUNT = 100;

void writeProcedure(LockFreeStack<int> *stack, const std::vector<int> &data) {
    for (int i = 0; i < data.size(); ++i) {
        stack->push(data[i]);
    }
}

void readProcedure(LockFreeStack<int> *stack, std::vector<int> *result_data) {
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
        int value = 0;
        while (!stack->pop(value)) 
            ;
        result_data->push_back(value); 
    }
}

TEST(LockFreeStack, many_threads) {
    testMultithreadContainer<LockFreeStack>(writeProcedure, 
            readProcedure, 
            THREADS_COUNT, 
            ELEMENTS_COUNT);
}
/*
    LockFreeStack<int> stack;
 
    std::vector<std::vector<int> > data = generateData(THREADS_COUNT, ELEMENTS_COUNT);
    std::vector<std::vector<int> > result_data(THREADS_COUNT);

    {
        std::vector<std::thread> writers;
        std::vector<std::thread> readers;

        for (int i = 0; i < THREADS_COUNT; ++i) {
            writers.emplace_back(std::thread(std::bind(writeProcedure, &stack, data[i])));
        }
        for (int i = 0; i < THREADS_COUNT; ++i) {
            writers[i].join();
        }
     
        for (int i = 0; i < THREADS_COUNT; ++i) {
            readers.emplace_back(std::thread(std::bind(readProcedure, &stack, &result_data[i])));
        }
        for (int i = 0; i < THREADS_COUNT; ++i) {
            readers[i].join();
        }
    }

    auto all_data = joinVectors(data);
    auto all_result_data = joinVectors(result_data);
    
    ASSERT_EQ(all_data.size(), all_result_data.size());
    for (int i = 0; i < all_data.size(); ++i) {
        EXPECT_EQ(all_data[i], all_result_data[i]);
    } 
}
*/
} // namespace tanyatik
