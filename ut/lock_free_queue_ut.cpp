#include <thread>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <random>

#include <gtest/gtest.h>

#include "../lock_free_queue.hpp"

namespace tanyatik {

TEST(LockFreeQueue, enqueue) {
    LockFreeQueue<int> queue;

    queue.enqueue(1);
    queue.enqueue(2);
}

TEST(LockFreeQueue, multithread_enqueue) {
    LockFreeQueue<int> queue;
    
    std::vector<std::thread> writers;
    const int WRITERS_COUNT = 100;

    for (int i = 0; i < WRITERS_COUNT; ++i) {
        writers.emplace_back(std::thread([&] () { queue.enqueue(1); }));
    }

    for (auto &writer: writers) {
        writer.join();
    }
    // EXPECT NO FAILURE
}
} // namespace tanyatik
