#include <gtest/gtest.h>

#include "../binary_heap.hpp"

typedef BinaryHeap<int> TestHeap;

void testHeap(const TestHeap& heap, 
              std::vector<typename TestHeap::TElementType> expected_elements) {
    auto heap_elements = heap.getElements();

    ASSERT_TRUE(heap.getSize() <= expected_elements.size());
    ASSERT_EQ(expected_elements.size(), heap_elements.size());


    for(size_t i = 0; i < heap.getSize(); ++i) {
        ASSERT_EQ(heap_elements[i], expected_elements[i]);
    }
}

TEST(heap_test, insertions) {
    TestHeap f(13);
    f.insert(20);
    testHeap(f, {20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    f.insert(15);
    testHeap(f, {20, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    f.insert(11);
    testHeap(f, {20, 15, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    f.insert(6);
    testHeap(f, {20, 15, 11, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    f.insert(9);
    testHeap(f, {20, 15, 11, 6, 9, 0, 0, 0, 0, 0, 0, 0, 0});
    f.insert(1);
    testHeap(f, {20, 15, 11, 6, 9, 1, 0, 0, 0, 0, 0, 0, 0});
    f.insert(3);
    testHeap(f, {20, 15, 11, 6, 9, 1, 3, 0, 0, 0, 0, 0, 0});
    f.insert(5);
    testHeap(f, {20, 15, 11, 6, 9, 1, 3, 5, 6, 0, 0, 0, 0});
    f.insert(17);
    testHeap(f, {20, 17, 11, 15, 9, 1, 3, 5, 6, 0, 0, 0, 0});
    f.insert(7);
    testHeap(f, {20, 17, 11, 15, 9, 1, 3, 5, 6, 7, 0, 0, 0});
    f.insert(8);
    testHeap(f, {20, 17, 11, 15, 9, 1, 3, 5, 6, 7, 8, 0, 0});
}

TEST(heap_test, extract_max) {
    TestHeap h(13);
    h.insert(20);
    h.insert(15);
    h.insert(11);
    h.insert(6);
    h.insert(9);
    h.insert(1);
    h.insert(3);
    h.insert(5);
    h.insert(17);
    h.insert(7);
    h.insert(8);

    testHeap(h, {20, 17, 11, 15, 9, 1, 3, 5, 6, 7, 8, 0, 0});
    ASSERT_EQ(h.getTop(), 20);
    h.pop();
    testHeap(h, {17, 15, 11, 8, 9, 1, 3, 5, 6, 7, 0, 0, 0});
    ASSERT_EQ(h.getTop(), 17);
    h.pop();
    testHeap(h, {15, 9, 11, 8, 7, 1, 3, 5, 6, 0, 0, 0, 0});
    ASSERT_EQ(h.getTop(), 15);
    h.pop();
    testHeap(h, {11, 9, 6, 8, 7, 1, 3, 5, 0, 0, 0, 0, 0});
    ASSERT_EQ(h.getTop(), 11);
    h.pop();
    testHeap(h, {9, 8, 6, 5, 7, 1, 3, 0, 0, 0, 0, 0, 0});
    ASSERT_EQ(h.getTop(), 9);
    h.pop();
    testHeap(h, {8, 7, 6, 5, 3, 1, 0, 0, 0, 0, 0, 0, 0});
    ASSERT_EQ(h.getTop(), 8);
    h.pop();
    testHeap(h, {7, 5, 6, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0});
    ASSERT_EQ(h.getTop(), 7);
    h.pop();
    testHeap(h, {6, 5, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    ASSERT_EQ(h.getTop(), 6);
    h.pop();
    testHeap(h, {5, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    ASSERT_EQ(h.getTop(), 5);
    h.pop();
    testHeap(h, {3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    ASSERT_EQ(h.getTop(), 3);
    h.pop();
    testHeap(h, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    ASSERT_EQ(h.getTop(), 1);
    h.pop();
    h.insert(20);
    testHeap(h, {20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    ASSERT_EQ(h.getTop(), 20);
}

TEST(heap_test, removals) {
    TestHeap h(15);
    h.insert(15);
    testHeap(h, {15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    h.removeElementByIndex(0);
    testHeap(h, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    h.insert(100);
    h.insert(25);
    h.insert(75);
    h.insert(15);
    h.insert(20);
    h.insert(55);
    h.insert(50);
    h.insert(11);
    h.insert(12);
    h.insert(13);
    h.insert(14);
    h.insert(52);
    h.insert(53);
    h.insert(48);
    h.insert(44);
    testHeap(h, {100, 25, 75, 15, 20, 55, 50, 11, 12, 13, 14, 52, 53, 48, 44});
    h.removeElementByIndex(1);
    testHeap(h, {100, 44, 75, 15, 20, 55, 50, 11, 12, 13, 14, 52, 53, 48, 0});
    h.removeElementByIndex(7);
    testHeap(h, {100, 48, 75, 44, 20, 55, 50, 15, 12, 13, 14, 52, 53, 0, 0});
    h.removeElementByIndex(0);

}

