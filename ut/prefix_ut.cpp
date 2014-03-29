#include <random>
#include <gtest/gtest.h>
#include <chrono>

#include "../prefix.hpp"
#include "test_helper.hpp"

namespace tanyatik {

TEST(prefix, prefix) {
    testVector({0, 0, 0, 0, 1, 2, 3}, computePrefix("abcdabc"));
    testVector({0, 0, 0, 0, 1, 2, 3, 1, 2, 3, 4, 5, 6, 7, 4, 5, 6}, 
                computePrefix("abcdabcabcdabcdab"));
}

TEST(prefix, edge_cases) {
    testVector({}, computePrefix(""));
    testVector({0}, computePrefix("a"));
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
   
    {
        timer_test timer;
        computeZFunction(input);
    }
}

TEST(pattern_matches, one_symbol_pattern) {
    testVector({}, getPatternMatches("a", "b"));
    testVector({0}, getPatternMatches("a", "a"));
    testVector({0, 1}, getPatternMatches("a", "aa"));
    testVector({0, 1, 2}, getPatternMatches("a", "aaa"));
    testVector({1}, getPatternMatches("a", "ba"));
}

TEST(pattern_matches, simple_pattern) {
    testVector({1, 3}, getPatternMatches("ab", "bababa"));
}

TEST(pattern_matches, regular_pattern) {
    testVector({0, 2}, getPatternMatches("aba", "ababa"));
    testVector({5, 14}, getPatternMatches("ababc", "abracababcdabrababcabra"));
    testVector({1}, getPatternMatches("abcab", "babcab"));
    testVector({1, 4}, getPatternMatches("abcab", "babcabcab"));
    testVector({2}, getPatternMatches("abcab", "ababcab"));
    testVector({7, 15}, getPatternMatches("abcab", "abracababcabrababcabra"));
    testVector({0}, getPatternMatches("aaa", "aaa"));
    testVector({0, 5, 7}, getPatternMatches("aba", "ababbababa"));
    testVector({0, 2, 6}, getPatternMatches("aba", "ababacaba"));
    testVector({2, 5, 8, 11, 18, 23}, getPatternMatches("abcab", "ababcabcabcabcabababcababcab"));
    testVector({4}, getPatternMatches("abddabaa", "abddabddabaaaba"));
    testVector({7}, getPatternMatches("babddbabee", "babddbababddbabee"));

    testVector({0}, getPatternMatches("abadabaa", "abadabaaaaaaaa"));
    testVector({4}, getPatternMatches("abadabaa", "abadabadabaaaa"));
    testVector({6}, getPatternMatches("abadabaa", "abadababadabaaa"));
}

TEST(pattern_matches, edge_cases) {
    testVector({}, getPatternMatches("abcd", "abc"));
    testVector({0, 1, 2, 3}, getPatternMatches("", "abcd"));
}

TEST(subpatterns, edge_cases) {
    testVector({{"", 0}}, getSubpatterns("???????????", '?'));
    testVector({{"", 0}}, getSubpatterns("?", '?'));
}

TEST(subpatterns, one_subpattern) {
    testVector({{"a", 1}}, getSubpatterns("?a", '?'));
    testVector({{"a", 3}}, getSubpatterns("???a", '?'));
    testVector({{"a", 0}}, getSubpatterns("a?", '?'));
    testVector({{"a", 0}}, getSubpatterns("a???", '?'));
    testVector({{"a", 1}}, getSubpatterns("?a?", '?'));
    testVector({{"a", 3}}, getSubpatterns("???a???", '?'));
}

TEST(subpatterns, many_subpatterns) {
    testVector({{"a", 1}, {"b", 3}}, getSubpatterns("?a?b", '?'));
    testVector({{"a", 1}, {"bba", 5}}, getSubpatterns("?a???bba", '?'));
}

TEST(subpattern, subpattern_occurence) {
    testVector({/*0 - 1,*/ 2 - 1}, getSubpatternMatches({"a", 1}, "aba")); // ?a
    testVector({/*0 - 3, */4 - 3, 8 - 3, 10 - 3}, 
            getSubpatternMatches({"aba", 3}, "abacabadababa")); // ???aba
}

TEST(intersection, intersection) {
    testVector({2, 3}, intersectVectors({{1, 2, 3}, {2, 3, 4}}));
    testVector({1, 3}, intersectVectors({{1, 2, 3}, {1, 3, 4}}));
    testVector({1, 2, 3}, intersectVectors({{1, 2, 3}, {1, 3, 4, 2}}));
    testVector({1, 2, 3}, intersectVectors({{0, 1, 2, 3, 5}, {1, 3, 4, 2}}));
    testVector({}, intersectVectors({{1, 2}, {3, 4}}));
}

TEST(fuzzy_pattern, fuzzy_pattern) {
    testVector({0, 1, 2}, getFuzzyPatternMatches("a", "aaa"));
    testVector({0}, getFuzzyPatternMatches("a?b", "aab"));
    testVector({0}, getFuzzyPatternMatches("a?b", "abb"));
    testVector({0}, getFuzzyPatternMatches("a??", "aaa"));
    testVector({0, 2, 5}, getFuzzyPatternMatches("ab?", "ababcabc"));
    testVector({0, 1, 2, 3, 4, 5}, getFuzzyPatternMatches("???", "ababcabc"));
}

} // namespace tanyatik
