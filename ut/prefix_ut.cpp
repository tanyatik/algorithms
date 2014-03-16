#include <random>
#include <gtest/gtest.h>

#include "../prefix.hpp"
#include "test_helper.hpp"

namespace tanyatik {

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

TEST(pattern_occurences, one_symbol_pattern) {
    testVector({}, getPatternOccurences("a", "b"));
    testVector({0}, getPatternOccurences("a", "a"));
    testVector({0, 1}, getPatternOccurences("a", "aa"));
    testVector({0, 1, 2}, getPatternOccurences("a", "aaa"));
    testVector({1}, getPatternOccurences("a", "ba"));
}

TEST(pattern_occurences, simple_pattern) {
    testVector({1, 3}, getPatternOccurences("ab", "bababa"));
}

TEST(pattern_occurences, regular_pattern) {
    testVector({0, 2}, getPatternOccurences("aba", "ababa"));
    testVector({5, 14}, getPatternOccurences("ababc", "abracababcdabrababcabra"));
    testVector({1}, getPatternOccurences("abcab", "babcab"));
    testVector({2}, getPatternOccurences("abcab", "ababcab"));
    testVector({7, 15}, getPatternOccurences("abcab", "abracababcabrababcabra"));
    testVector({0}, getPatternOccurences("aaa", "aaa"));
    testVector({0, 5, 7}, getPatternOccurences("aba", "ababbababa"));
}

TEST(pattern_occurences, edge_cases) {
    testVector({}, getPatternOccurences("abcd", "abc"));
}

TEST(pattern_occurences, time) {
    const int test_size = 50000;
    std::string pattern(test_size * 0.3, 0);
    std::string text(test_size, 0);
    
    std::vector<char> alphabet = {'a', 'b', 'c', 'd', 'e' };
    static std::default_random_engine g_random_engine;

    int min = 0;
    int max = alphabet.size() - 1;

    for (size_t i = 0; i < pattern.size(); ++i) {
        int letter_index = std::uniform_int_distribution<int>(min, max) (g_random_engine);
        pattern[i] = alphabet[letter_index];
    }
    
    for (size_t i = 0; i < text.size(); ++i) {
        int letter_index = std::uniform_int_distribution<int>(min, max) (g_random_engine);
        text[i] = alphabet[letter_index];
    }

    getPatternOccurences(pattern, text);
}

} // namespace tanyatik
