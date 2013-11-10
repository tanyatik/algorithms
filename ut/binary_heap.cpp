#include <gtest/gtest.h>

#include "../binary_heap.hpp"

template<>
class TraitsSentinel<int> {
    public:
        enum {
            MINUS_SENTINEL = -10000000
        };
};

typedef BinaryHeap<int, long long> TestHeap;

static TestHeap constructTestHeap() {
    TestHeap f(13);
    f.insert(20, 1);
    f.insert(15, 2);
    f.insert(11, 3);
    f.insert(6, 4);
    f.insert(9, 5);
    f.insert(1, 6);
    f.insert(3, 7);
    f.insert(5, 8);
    f.insert(17, 9);
    f.insert(7, 10);
    f.insert(8, 11);
    return f;
}

void testHeap(const TestHeap& heap, 
              std::vector<typename TestHeap::TKeyType> exp_keys, 
              std::vector<typename TestHeap::TValueType> exp_values) {
    auto heap_keys = heap.getKeys();
    auto heap_values = heap.getValues();

    ASSERT_TRUE(heap.getSize() <= exp_keys.size());
    ASSERT_TRUE(heap.getSize() <= exp_values.size());
    ASSERT_TRUE(exp_keys.size() == exp_values.size());
    ASSERT_TRUE(exp_keys.size() == heap_keys.size());
    ASSERT_TRUE(exp_values.size() == heap_values.size());


    for(size_t i = 0; i < heap.getSize(); ++i) {
        ASSERT_EQ(heap_keys[i], exp_keys[i]);
        ASSERT_EQ(heap_values[i], exp_values[i]);
    }
}

TEST(heap_test, insertions) {
    TestHeap f(13);
    f.insert(20, 1);
    testHeap(f, {20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    f.insert(15, 2);
    testHeap(f, {20, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    f.insert(11, 3);
    testHeap(f, {20, 15, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    f.insert(6, 4);
    testHeap(f, {20, 15, 11, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    f.insert(9, 5);
    testHeap(f, {20, 15, 11, 6, 9, 0, 0, 0, 0, 0, 0, 0, 0}, {1, 2, 3, 4, 5, 0, 0, 0, 0, 0, 0, 0, 0});
    f.insert(1, 6);
    testHeap(f, {20, 15, 11, 6, 9, 1, 0, 0, 0, 0, 0, 0, 0}, {1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0});
    f.insert(3, 7);
    testHeap(f, {20, 15, 11, 6, 9, 1, 3, 0, 0, 0, 0, 0, 0}, {1, 2, 3, 4, 5, 6, 7, 0, 0, 0, 0, 0, 0});
    f.insert(5, 8);
    testHeap(f, {20, 15, 11, 6, 9, 1, 3, 5, 6, 0, 0, 0, 0}, {1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0});
    f.insert(17, 9);
    testHeap(f, {20, 17, 11, 15, 9, 1, 3, 5, 6, 0, 0, 0, 0}, {1, 9, 3, 2, 5, 6, 7, 8, 4, 0, 0, 0, 0});
    f.insert(7, 10);
    testHeap(f, {20, 17, 11, 15, 9, 1, 3, 5, 6, 7, 0, 0, 0}, {1, 9, 3, 2, 5, 6, 7, 8, 4, 10, 0, 0, 0});
    f.insert(8, 11);
    testHeap(f, {20, 17, 11, 15, 9, 1, 3, 5, 6, 7, 8, 0, 0}, {1, 9, 3, 2, 5, 6, 7, 8, 4, 10, 11, 0, 0});
}

TEST(heap_test, extract_max) {
    auto h = constructTestHeap();
    testHeap(h, {20, 17, 11, 15, 9, 1, 3, 5, 6, 7, 8, 0, 0}, {1, 9, 3, 2, 5, 6, 7, 8, 4, 10, 11, 0, 0});
    ASSERT_EQ(h.extractMax(), 1);
    testHeap(h, {17, 15, 11, 8, 9, 1, 3, 5, 6, 7, 0, 0, 0}, {9, 2, 3, 11, 5, 6, 7, 8, 4, 10, 0, 0, 0});
    ASSERT_EQ(h.extractMax(), 9);
    testHeap(h, {15, 8, 11, 7, 9, 1, 3, 5, 6, 0, 0, 0, 0}, {2, 11, 3, 10, 5, 6, 7, 8, 4, 0, 0, 0, 0});
    ASSERT_EQ(h.extractMax(), 2);
    testHeap(h, {8, 7, 11, 6, 9, 1, 3, 5, 0, 0, 0, 0, 0}, {11, 10, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0, 0});
    ASSERT_EQ(h.extractMax(), 11);
    testHeap(h, {7, 6, 11, 5, 9, 1, 3, 0, 0, 0, 0, 0, 0}, {10, 4, 3, 8, 5, 6, 7, 0, 0, 0, 0, 0, 0});
    ASSERT_EQ(h.extractMax(), 10);
    testHeap(h, {6, 5, 11, 3, 9, 1, 0, 0, 0, 0, 0, 0, 0}, {4, 8, 3, 7, 5, 6, 0, 0, 0, 0, 0, 0, 0});
    ASSERT_EQ(h.extractMax(), 4);
    testHeap(h, {5, 3, 11, 1, 9, 0, 0, 0, 0, 0, 0, 0, 0}, {8, 7, 3, 6, 5, 0, 0, 0, 0, 0, 0, 0, 0});
    ASSERT_EQ(h.extractMax(), 8);
    testHeap(h, {3, 9, 11, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {7, 5, 3, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    ASSERT_EQ(h.extractMax(), 7);
    testHeap(h, {9, 1, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {5, 6, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    ASSERT_EQ(h.extractMax(), 5);
    testHeap(h, {11, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {3, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    ASSERT_EQ(h.extractMax(), 3);
    testHeap(h, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    ASSERT_EQ(h.extractMax(), 6);
    h.insert(20, 1);
    testHeap(h, {20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    ASSERT_EQ(h.extractMax(), 1);
}

GTEST_API_ int main(int argc, char **argv) {
    printf("Running main() from gtest_main.cc\n");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
