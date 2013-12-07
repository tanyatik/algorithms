#include <gtest/gtest.h>
#include <unordered_set>
#include <fstream>

#include "../hash_set.hpp"

TEST(hash_table, universal_hash_function) {
    UniversalHashFunctor f = generateUniversalHashFunctor(2000000011);
    UniversalHashFunctor g = generateUniversalHashFunctor(2000000011);

    int k = 12345;
    ASSERT_TRUE(f(k) != g(k));
}

TEST(hash_set, initialization) {
    FixedSet f;

    std::vector<int> keys {1, 2, 4, 5, 7, 11, 28};
    f.initialize(keys);
    ASSERT_TRUE(f.contains(11));
    ASSERT_TRUE(f.contains(1));
    ASSERT_TRUE(f.contains(2));
    ASSERT_FALSE(f.contains(3));
    ASSERT_TRUE(f.contains(4));
    ASSERT_TRUE(f.contains(5));
    ASSERT_FALSE(f.contains(6));
    ASSERT_TRUE(f.contains(7));
    ASSERT_FALSE(f.contains(8));
    ASSERT_FALSE(f.contains(9));
    ASSERT_FALSE(f.contains(10));
    ASSERT_FALSE(f.contains(29));
}

TEST(hash_set, initialization_big) {
    FixedSet f;

    static std::default_random_engine g_random_engine;
    std::unordered_set<int> set;
    std::vector<int> keys;
    for (int i = 0; i < 100000; ++i) {
        int element = std::uniform_int_distribution<int>(-1000000000, 1000000000) (g_random_engine);
        set.insert(element);
        keys.push_back(element);
    }

    f.initialize(keys);
    for (int i = -1000; i <= 1000; ++i) {
        ASSERT_TRUE((f.contains(i)) == (set.find(i) != set.end()));
    }
}

