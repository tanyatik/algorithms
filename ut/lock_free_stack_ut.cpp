#include <thread>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <random>

#include <gtest/gtest.h>

#include "../lock_free_stack.hpp"

TEST(lock_free_stack_t, push_pop) {
    lock_free_stack_t<int> stack;
    int value;

    stack.push(3);
    stack.pop(value); 

    EXPECT_EQ(3, value);
}

TEST(lock_free_stack_t, push_push_pop_pop) {
    lock_free_stack_t<int> stack;
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

void write_procedure(lock_free_stack_t<int> *stack, const std::vector<int> &data) {
    for (int i = 0; i < data.size(); ++i) {
        stack->push(data[i]);
        //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void read_procedure(lock_free_stack_t<int> *stack, std::vector<int> *result_data) {
    for (int i = 0; i < ELEMENTS_COUNT; ++i) {
        int value = 0;
        while (!stack->pop(value)) 
            ;
        result_data->push_back(value); 
        // int delete_list_size = stack->get_delete_queue_size();
        // if (delete_list_size > 0) {
        //    std::cout << "delete_list_ size: " << delete_list_size << std::endl;
        // }
    }
}

std::vector<int> join_vectors(const std::vector<std::vector<int> > &vectors) {
    std::vector<int> joined;
    for (auto &vector: vectors) {
        joined.insert(joined.end(), vector.begin(), vector.end());
    }

    std::sort(joined.begin(), joined.end());

    return std::move(joined);
}

std::vector<std::vector<int> > generate_data() {
    std::vector<std::vector<int> > data(THREADS_COUNT, std::vector<int> (ELEMENTS_COUNT));
    std::default_random_engine generator;

    for (auto &thread_data: data) {
        for (auto &data_element: thread_data) {
            data_element = std::uniform_int_distribution<int>(0, 100)(generator);
        }
    }

    return std::move(data);
}

TEST(lock_free_stack_t, many_threads) {
    lock_free_stack_t<int> stack;
 
    std::vector<std::vector<int> > data = generate_data();
    std::vector<std::vector<int> > result_data(THREADS_COUNT);

    {
        std::vector<std::thread> writers;
        std::vector<std::thread> readers;

        for (int i = 0; i < THREADS_COUNT; ++i) {
            writers.emplace_back(std::thread(std::bind(write_procedure, &stack, data[i])));
        }
        for (int i = 0; i < THREADS_COUNT; ++i) {
            writers[i].join();
        }
     
        for (int i = 0; i < THREADS_COUNT; ++i) {
            readers.emplace_back(std::thread(std::bind(read_procedure, &stack, &result_data[i])));
        }
        for (int i = 0; i < THREADS_COUNT; ++i) {
            readers[i].join();
        }
    }

    auto all_data = join_vectors(data);
    auto all_result_data = join_vectors(result_data);
    
    ASSERT_EQ(all_data.size(), all_result_data.size());
    for (int i = 0; i < all_data.size(); ++i) {
        EXPECT_EQ(all_data[i], all_result_data[i]);
    } 
}
