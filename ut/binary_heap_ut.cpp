#include <gtest/gtest.h>

#include "../binary_heap.hpp"

typedef algorithms::BinaryHeap<int> TestHeap;


TEST(heap_test, insert_extract_max) {
    TestHeap h;
    h.Insert(20);
    h.Insert(15);
    h.Insert(11);
    h.Insert(6);
    h.Insert(9);
    h.Insert(1);
    h.Insert(3);
    h.Insert(5);
    h.Insert(17);
    h.Insert(7);
    h.Insert(8);

    ASSERT_EQ(h.GetTop(), 20);
    h.Pop();
    ASSERT_EQ(h.GetTop(), 17);
    h.Pop();
    ASSERT_EQ(h.GetTop(), 15);
    h.Pop();
    ASSERT_EQ(h.GetTop(), 11);
    h.Pop();
    ASSERT_EQ(h.GetTop(), 9);
    h.Pop();
    ASSERT_EQ(h.GetTop(), 8);
    h.Pop();
    ASSERT_EQ(h.GetTop(), 7);
    h.Pop();
    ASSERT_EQ(h.GetTop(), 6);
    h.Pop();
    ASSERT_EQ(h.GetTop(), 5);
    h.Pop();
    ASSERT_EQ(h.GetTop(), 3);
    h.Pop();
    ASSERT_EQ(h.GetTop(), 1);
    h.Pop();
    h.Insert(20);
    ASSERT_EQ(h.GetTop(), 20);
}

TEST(heap_test, removals) {
    TestHeap h;
    h.Insert(15);
    ASSERT_EQ(h.GetTop(), 15);
    h.RemoveElementByIndex(0);
    ASSERT_EQ(h.GetSize(), 0);
    h.Insert(100);
    h.Insert(25);
    h.Insert(75);
    h.Insert(15);
    h.Insert(20);
    h.Insert(55);
    h.Insert(50);
    h.Insert(11);
    h.Insert(12);
    h.Insert(13);
    h.Insert(14);
    h.Insert(52);
    h.Insert(53);
    h.Insert(48);
    h.Insert(44);
    ASSERT_EQ(h.GetTop(), 100);
    h.RemoveElementByIndex(1);
    ASSERT_EQ(h.GetTop(), 100);
    h.RemoveElementByIndex(7);
    h.RemoveElementByIndex(0);
    ASSERT_EQ(h.GetTop(), 75);
}

