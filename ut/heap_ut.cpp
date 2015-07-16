#include <gtest/gtest.h>

#include <unordered_set>

#include "heap/binary_heap.hpp"
#include "heap/treap.hpp"


TEST(binary_heap_test, insert_extract_max) {
    typedef algorithms::BinaryHeap<int> TestHeap;

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

TEST(binary_heap_test, removals) {
    typedef algorithms::BinaryHeap<int> TestHeap;
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

// Treap
typedef algorithms::Treap<int> TestTreap;

bool CheckTreap(const TestTreap &treap) {
    try {
        treap.CheckStructure();

    } catch (const std::runtime_error &exception) {
        std::cout << exception.what();
        return false;

    }
    return true;
}

TEST(treap, Insert) {
    TestTreap test;
    ASSERT_TRUE(test.Insert(5));
    ASSERT_TRUE(CheckTreap(test));
    ASSERT_TRUE(test.Insert(3));
    ASSERT_TRUE(CheckTreap(test));
    ASSERT_TRUE(test.Insert(19234));
    ASSERT_TRUE(CheckTreap(test));

    ASSERT_FALSE(test.Insert(19234));
    ASSERT_TRUE(CheckTreap(test));
    ASSERT_FALSE(test.Insert(5));
    ASSERT_TRUE(CheckTreap(test));
    ASSERT_FALSE(test.Insert(3));
    ASSERT_TRUE(CheckTreap(test));


    ASSERT_TRUE(test.Insert(243));
    ASSERT_TRUE(CheckTreap(test));
    ASSERT_TRUE(test.Insert(634379));
    ASSERT_TRUE(CheckTreap(test));
    ASSERT_TRUE(test.Insert(1234));
    ASSERT_TRUE(CheckTreap(test));
}

TEST(treap, Insert_stress) {
    TestTreap test;

    static std::default_random_engine g_random_engine;
    std::unordered_set<int> set;
    for (int i = 0; i < 100; ++i) {
        int min = 0;
        int max = std::numeric_limits<int>::max();

        int element = std::uniform_int_distribution<int>(min, max) (g_random_engine);
        set.insert(element);
    }

    for (auto set_element : set) {
        ASSERT_TRUE(test.Insert(set_element));
        ASSERT_TRUE(CheckTreap(test));
    }

    for (auto set_element : set) {
        ASSERT_FALSE(test.Insert(set_element));
        ASSERT_TRUE(CheckTreap(test));
    }
}

TEST(treap, erase) {
    TestTreap test;
    ASSERT_TRUE(test.Insert(5));
    ASSERT_TRUE(CheckTreap(test));
    ASSERT_TRUE(test.Insert(3));
    ASSERT_TRUE(CheckTreap(test));
    ASSERT_TRUE(test.Insert(19234));
    ASSERT_TRUE(CheckTreap(test));

    ASSERT_TRUE(test.Erase(19234));
    ASSERT_TRUE(CheckTreap(test));
    ASSERT_TRUE(test.Erase(3));
    ASSERT_TRUE(CheckTreap(test));
    ASSERT_TRUE(test.Erase(5));
    ASSERT_TRUE(CheckTreap(test));

    ASSERT_FALSE(test.Erase(19234));
    ASSERT_TRUE(CheckTreap(test));
    ASSERT_FALSE(test.Erase(5));
    ASSERT_TRUE(CheckTreap(test));
    ASSERT_FALSE(test.Erase(3));
    ASSERT_TRUE(CheckTreap(test));
}

TEST(treap, erase_stress) {
    TestTreap test;

    static std::default_random_engine g_random_engine;
    std::unordered_set<int> set;
    for (int i = 0; i < 100; ++i) {
        int min = 0;
        int max = std::numeric_limits<int>::max();

        int element = std::uniform_int_distribution<int>(min, max) (g_random_engine);
        set.insert(element);
    }

    for (auto set_element : set) {
        ASSERT_TRUE(test.Insert(set_element));
        ASSERT_TRUE(CheckTreap(test));
    }

    for (auto set_element : set) {
        ASSERT_TRUE(test.Erase(set_element));
        ASSERT_TRUE(CheckTreap(test));
    }

    for (auto set_element : set) {
        ASSERT_FALSE(test.Erase(set_element));
        ASSERT_TRUE(CheckTreap(test));
    }
}

TEST(treap, find_this_or_next) {
    TestTreap test;

    for (int i = 0; i < 5; ++i) {
        test.Insert(2 * i + 1);
    }

    int result = 0;
    for (int i = 0; i < 5; ++i) {
        test.FindThisOrNext(2 * i, &result);
        ASSERT_EQ(2 * i + 1, result);
        test.FindThisOrNext(2 * i + 1, &result);
        ASSERT_EQ(2 * i + 1, result);
    }
    ASSERT_FALSE(test.FindThisOrNext(10, &result));
}
