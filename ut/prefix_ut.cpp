#include <random>
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
    testVector({5, 4, 3, 2, 1}, computeZFunction("aaaaa"));
    testVector({5, 0, 0, 2, 0}, computeZFunction("abcab"));
    testVector({7, 0, 1, 0, 3, 0, 1}, computeZFunction("abacaba"));
    testVector({9, 0, 0, 0, 0, 4, 0, 0, 0}, computeZFunction("abcdeabcd"));
}

TEST(z_function, complicated) {
    testVector({16, 0, 0, 0, 2, 0, 0, 0, 6, 0, 0, 0, 2, 0, 0, 1}, computeZFunction("abcdabscabcdabia"));
    testVector({11, 0, 0, 1, 0, 1, 0, 4, 0, 0, 1}, computeZFunction("abracadabra"));
}

TEST(z_function, time) {
    const int test_size = 1000000;
    std::string input(test_size, 0);
    
    std::vector<char> alphabet = {'a', 'b', 'c', 'd', 'e' };
    static std::default_random_engine g_random_engine;

    for (int i = 0; i < test_size; ++i) {
        int min = 0;
        int max = alphabet.size() - 1;

        int letter_index = std::uniform_int_distribution<int>(min, max) (g_random_engine);
        input[i] = alphabet[letter_index];
    }
    
    computeZFunction(input);
}
