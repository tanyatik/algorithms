#include <gtest/gtest.h>

#include "../prefix.hpp"
#include "test_helper.hpp"

using tanyatik::computePrefix;
using tanyatik::computeZFunction;

TEST(prefix, simple) {
    testVector({0, 0, 0, 0, 1, 2, 3}, computePrefix("abcdabc"));
}

TEST(prefix, complicated) {
    testVector({0, 0, 0, 0, 1, 2, 3, 1, 2, 3, 4, 5, 6, 7, 4, 5, 6}, 
                computePrefix("abcdabcabcdabcdab"));
} 

TEST(z_function, simple) {
    testVector({7, 0, 1, 0, 3, 0, 1}, computeZFunction("abacaba"));
}

TEST(z_function, complicated) {
    testVector({16, 0, 0, 0, 2, 0, 0, 0, 6, 0, 0, 0, 2, 0, 0, 1}, computeZFunction("abcdabscabcdabia"));
}
