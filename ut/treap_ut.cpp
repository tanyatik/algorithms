#include <gtest/gtest.h>
#include <unordered_set>

#include "../treap.hpp"

typedef tanyatik::Treap<int> TestTreap;

bool checkTreap(const TestTreap &treap) {
    try {
        treap.checkStructure();

    } catch (const std::runtime_error &exception) {
        std::cout << exception.what();
        return false;

    }
    return true;
}

TEST(treap, insert) {
    TestTreap test;
    ASSERT_TRUE(test.insert(5));
    ASSERT_TRUE(checkTreap(test));
    ASSERT_TRUE(test.insert(3));
    ASSERT_TRUE(checkTreap(test));
    ASSERT_TRUE(test.insert(19234));
    ASSERT_TRUE(checkTreap(test));

    ASSERT_FALSE(test.insert(19234));
    ASSERT_TRUE(checkTreap(test));
    ASSERT_FALSE(test.insert(5));
    ASSERT_TRUE(checkTreap(test));
    ASSERT_FALSE(test.insert(3));
    ASSERT_TRUE(checkTreap(test));


    ASSERT_TRUE(test.insert(243));
    ASSERT_TRUE(checkTreap(test));
    ASSERT_TRUE(test.insert(634379));
    ASSERT_TRUE(checkTreap(test));
    ASSERT_TRUE(test.insert(1234));
    ASSERT_TRUE(checkTreap(test));
}

TEST(treap, insert_stress) {
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
        ASSERT_TRUE(test.insert(set_element));
        ASSERT_TRUE(checkTreap(test));
    }

    for (auto set_element : set) {
        ASSERT_FALSE(test.insert(set_element));
        ASSERT_TRUE(checkTreap(test));
    }
}

TEST(treap, erase) {
    TestTreap test;
    ASSERT_TRUE(test.insert(5));
    ASSERT_TRUE(checkTreap(test));
    ASSERT_TRUE(test.insert(3));
    ASSERT_TRUE(checkTreap(test));
    ASSERT_TRUE(test.insert(19234));
    ASSERT_TRUE(checkTreap(test));

    ASSERT_TRUE(test.erase(19234));
    ASSERT_TRUE(checkTreap(test));
    ASSERT_TRUE(test.erase(3));
    ASSERT_TRUE(checkTreap(test));
    ASSERT_TRUE(test.erase(5));
    ASSERT_TRUE(checkTreap(test));

    ASSERT_FALSE(test.erase(19234));
    ASSERT_TRUE(checkTreap(test));
    ASSERT_FALSE(test.erase(5));
    ASSERT_TRUE(checkTreap(test));
    ASSERT_FALSE(test.erase(3));
    ASSERT_TRUE(checkTreap(test));
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
        ASSERT_TRUE(test.insert(set_element));
        ASSERT_TRUE(checkTreap(test));
    }

    for (auto set_element : set) {
        ASSERT_TRUE(test.erase(set_element));
        ASSERT_TRUE(checkTreap(test));
    }

    for (auto set_element : set) {
        ASSERT_FALSE(test.erase(set_element));
        ASSERT_TRUE(checkTreap(test));
    }
}

TEST(treap, find_this_or_next) {
    TestTreap test;

    for (int i = 0; i < 5; ++i) {
        test.insert(2 * i + 1);
    }

    int result = 0;
    for (int i = 0; i < 5; ++i) {
        test.findThisOrNext(2 * i, &result);
        ASSERT_EQ(2 * i + 1, result);
        test.findThisOrNext(2 * i + 1, &result);
        ASSERT_EQ(2 * i + 1, result);
    }
    ASSERT_FALSE(test.findThisOrNext(10, &result));
}
