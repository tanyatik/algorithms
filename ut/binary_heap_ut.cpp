#include <gtest/gtest.h>

#include "../binary_heap.hpp"

typedef BinaryHeap<int> TestHeap;


TEST(heap_test, insert_extract_max) {
    TestHeap h;
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

    ASSERT_EQ(h.getTop(), 20);
    h.pop();
    ASSERT_EQ(h.getTop(), 17);
    h.pop();
    ASSERT_EQ(h.getTop(), 15);
    h.pop();
    ASSERT_EQ(h.getTop(), 11);
    h.pop();
    ASSERT_EQ(h.getTop(), 9);
    h.pop();
    ASSERT_EQ(h.getTop(), 8);
    h.pop();
    ASSERT_EQ(h.getTop(), 7);
    h.pop();
    ASSERT_EQ(h.getTop(), 6);
    h.pop();
    ASSERT_EQ(h.getTop(), 5);
    h.pop();
    ASSERT_EQ(h.getTop(), 3);
    h.pop();
    ASSERT_EQ(h.getTop(), 1);
    h.pop();
    h.insert(20);
    ASSERT_EQ(h.getTop(), 20);
}

TEST(heap_test, removals) {
    TestHeap h;
    h.insert(15);
    ASSERT_EQ(h.getTop(), 15);
    h.removeElementByIndex(0);
    ASSERT_EQ(h.getSize(), 0);
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
    ASSERT_EQ(h.getTop(), 100);
    h.removeElementByIndex(1);
    ASSERT_EQ(h.getTop(), 100);
    h.removeElementByIndex(7);
    h.removeElementByIndex(0);
    ASSERT_EQ(h.getTop(), 75);
}

