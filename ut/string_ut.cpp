#include <gtest/gtest.h>

#include "string/aho_corasik.hpp"
#include "string/prefix.hpp"
#include "string/suffix_automata.hpp"

#include "test_helper.hpp"

using namespace algorithms;

// Aho-Corasik
using namespace std::placeholders;

TEST(aho_corasik, trie_fill) {
    Trie<int> trie;

    trie.AddKey("abc", 1);
    trie.AddKey("def", 2);

    EXPECT_TRUE(trie.HasKey("abc"));
    EXPECT_TRUE(trie.HasKey("def"));

    EXPECT_FALSE(trie.HasKey("abd"));
    EXPECT_FALSE(trie.HasKey("ab"));
    EXPECT_FALSE(trie.HasKey("abcd"));

    trie.AddKey("ab", 3);
    trie.AddKey("deg", 4);
    trie.AddKey("degh", 5);

    EXPECT_TRUE(trie.HasKey("abc"));
    EXPECT_TRUE(trie.HasKey("ab"));
    EXPECT_TRUE(trie.HasKey("def"));
    EXPECT_TRUE(trie.HasKey("deg"));
    EXPECT_TRUE(trie.HasKey("degh"));

    EXPECT_FALSE(trie.HasKey("abd"));
    EXPECT_FALSE(trie.HasKey("abcd"));
    EXPECT_FALSE(trie.HasKey("de"));
}

TEST(aho_corasik, empty_trie_fill) {
    Trie<int> trie;
    trie.AddKey("", 1);

    EXPECT_TRUE(trie.HasKey(""));
    EXPECT_FALSE(trie.HasKey("notintrie"));
}

typedef std::map<std::string, std::set<size_t>> ExpectedMap;

ExpectedMap GetResultMap(const std::vector<std::vector<size_t>> &result,
        const std::vector<std::string> patterns) {
    ExpectedMap result_map;

    assert(result.size() == patterns.size());
    for (size_t result_index = 0; result_index < result.size(); ++result_index) {
        const auto &matches = result[result_index];
        if (!matches.empty()) {
            result_map[patterns[result_index]].insert(matches.begin(), matches.end());
        }
    }

    return std::move(result_map);
}

void CheckMap(ExpectedMap expected,
        std::vector<std::vector<size_t>> result_matches,
        const std::vector<std::string> patterns) {
    ExpectedMap result = GetResultMap(result_matches, patterns);

    EXPECT_EQ(expected.size(), result.size()) << "not all matches found";
    for (auto match: expected) {
        auto &expected_matches = expected[match.first];
        auto &result_matches = result[match.first];

        EXPECT_EQ(expected_matches.size(), result_matches.size()) << "wrong size of matches for "
            << match.first << std::endl;

        for (auto expected_match: expected_matches) {
            EXPECT_TRUE(result_matches.find(expected_match) != result_matches.end())
                << " not matched " << match.first <<
                " at position " << expected_match << std::endl;
        }
    }
}

void CheckPatternsMatching(const std::string &text,
        const std::vector<std::string> &patterns,
        ExpectedMap expected_map) {
    PatternsMatcher matcher;
    matcher.Init(patterns);

    auto found_map = matcher.SearchPatterns(text);
    CheckMap(expected_map, found_map, patterns);
}

TEST(aho_corasik, empty_pattern_matcher_fill) {
    CheckPatternsMatching("ababba", {"", "a"}, {{"a", {0, 2, 5}}, {"", {0, 1, 2, 3, 4, 5}}});
}

TEST(aho_corasik, single_pattern_matcher_fill) {
    CheckPatternsMatching("aba", {"ab"}, {{"ab", {1}}});
}

TEST(aho_corasik, overlapping_pattern_matcher_fill) {
    CheckPatternsMatching("herhehis",
        {"he", "she", "his", "her"},
        {{"his", {7}}, {"he", {1, 4}}, {"her", {2}}});
    CheckPatternsMatching("hipsher",
        {"he", "she", "i", "ip", "ips", "hips", "her", "h"},
        {{"h",    {0, 4}},
         {"i",    {1}},
         {"ip",   {2}},
         {"hips", {3}},
         {"ips",  {3}},
         {"she",  {5}},
         {"he",   {5}},
         {"her",  {6}}});
}

TEST(aho_corasik, long_string_matcher_fill) {
    CheckPatternsMatching("abcdcbcddbbbcccbbbcccbb",
        {"abc", "bcdc", "bcdd", "bbbc", "cccb"},
        {{"abc",  {2}},
         {"bcdc", {4}},
         {"bcdd", {8}},
         {"bbbc", {12, 18}},
         {"cccb", {15, 21}}});
}

TEST(word_counter, with_2_letters_of_length_5_except_a) {
    StringsCounter counter;
    counter.Init({"a"});

    EXPECT_EQ(1, counter.CountExcludedStrings(5, 2));
}

TEST(word_counter, with_1_letter_of_length_5_except_a_aa) {
    StringsCounter counter;
    counter.Init({"a", "aa"});

    EXPECT_EQ(0, counter.CountExcludedStrings(5, 1));
}

TEST(word_counter, with_2_letters_of_length_5_except_ab) {
    StringsCounter counter;
    counter.Init({"ab"});

    EXPECT_EQ(6, counter.CountExcludedStrings(5, 2));
}

TEST(word_counter, with_2_letters_of_length_5) {
    StringsCounter counter;
    counter.Init({});

    EXPECT_EQ(32, counter.CountExcludedStrings(5, 2));
}

// prefix
TEST(prefix, simple) {
    TestVector({0, 0, 0, 0, 1, 2, 3}, ComputePrefix("abcdabc"));
}

TEST(prefix, complicated) {
    TestVector({0, 0, 0, 0, 1, 2, 3, 1, 2, 3, 4, 5, 6, 7, 4, 5, 6},
                ComputePrefix("abcdabcabcdabcdab"));
}

TEST(z_function, simple) {
    TestVector({5, 4, 3, 2, 1}, ComputeZFunction("aaaaa"));
    TestVector({5, 0, 0, 2, 0}, ComputeZFunction("abcab"));
    TestVector({7, 0, 1, 0, 3, 0, 1}, ComputeZFunction("abacaba"));
    TestVector({9, 0, 0, 0, 0, 4, 0, 0, 0}, ComputeZFunction("abcdeabcd"));
}

TEST(z_function, complicated) {
    TestVector({16, 0, 0, 0, 2, 0, 0, 0, 6, 0, 0, 0, 2, 0, 0, 1}, ComputeZFunction("abcdabscabcdabia"));
    TestVector({11, 0, 0, 1, 0, 1, 0, 4, 0, 0, 1}, ComputeZFunction("abracadabra"));
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

    ComputeZFunction(input);
}

// suffix automata
TEST(suffix_automata, count_different_substrings_ab) {
    SuffixAutomata s;
    s.Init("ab");

    EXPECT_EQ(3, s.CountDifferentSubstrings());
}

TEST(suffix_automata, count_different_substrings_abcbd) {
    SuffixAutomata s;
    s.Init("abcbd");

    EXPECT_EQ(14, s.CountDifferentSubstrings());
}
