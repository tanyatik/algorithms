#include <gtest/gtest.h>

#include "../aho_corasik.hpp"

using namespace std::placeholders;

TEST(aho_corasik, trie_fill) {
    Trie<int> trie;

    trie.addKey("abc", 1);
    trie.addKey("def", 2);

    EXPECT_TRUE(trie.hasKey("abc"));
    EXPECT_TRUE(trie.hasKey("def"));

    EXPECT_FALSE(trie.hasKey("abd"));
    EXPECT_FALSE(trie.hasKey("ab"));
    EXPECT_FALSE(trie.hasKey("abcd"));
    
    trie.addKey("ab", 3);
    trie.addKey("deg", 4);
    trie.addKey("degh", 5);
     
    EXPECT_TRUE(trie.hasKey("abc"));
    EXPECT_TRUE(trie.hasKey("ab"));
    EXPECT_TRUE(trie.hasKey("def"));
    EXPECT_TRUE(trie.hasKey("deg"));
    EXPECT_TRUE(trie.hasKey("degh"));

    EXPECT_FALSE(trie.hasKey("abd"));
    EXPECT_FALSE(trie.hasKey("abcd"));
    EXPECT_FALSE(trie.hasKey("de"));
}

TEST(aho_corasik, empty_trie_fill) {
    Trie<int> trie;
    trie.addKey("", 1);

    EXPECT_TRUE(trie.hasKey(""));
    EXPECT_FALSE(trie.hasKey("notintrie"));
}

typedef std::map<std::string, std::set<size_t>> ExpectedMap;

ExpectedMap getResultMap(const std::vector<std::vector<size_t>> &result,
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

void checkMap(ExpectedMap expected, 
        std::vector<std::vector<size_t>> result_matches, 
        const std::vector<std::string> patterns) {
    ExpectedMap result = getResultMap(result_matches, patterns);

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

void checkPatternsMatching(const std::string &text, 
        const std::vector<std::string> &patterns,
        ExpectedMap expected_map) {
    PatternsMatcher matcher;
    matcher.init(patterns);

    auto found_map = matcher.searchPatterns(text);
    checkMap(expected_map, found_map, patterns);
}

TEST(aho_corasik, empty_pattern_matcher_fill) {
    checkPatternsMatching("ababba", {"", "a"}, {{"a", {0, 2, 5}}, {"", {0, 1, 2, 3, 4, 5}}});
}

TEST(aho_corasik, single_pattern_matcher_fill) {
    checkPatternsMatching("aba", {"ab"}, {{"ab", {1}}});
}

TEST(aho_corasik, overlapping_pattern_matcher_fill) {
    checkPatternsMatching("herhehis", 
        {"he", "she", "his", "her"}, 
        {{"his", {7}}, {"he", {1, 4}}, {"her", {2}}});
    checkPatternsMatching("hipsher",
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
    checkPatternsMatching("abcdcbcddbbbcccbbbcccbb", 
        {"abc", "bcdc", "bcdd", "bbbc", "cccb"},
        {{"abc",  {2}}, 
         {"bcdc", {4}}, 
         {"bcdd", {8}}, 
         {"bbbc", {12, 18}}, 
         {"cccb", {15, 21}}});
}

TEST(word_counter, with_2_letters_of_length_5_except_a) {
    StringsCounter counter;
    counter.init({"a"});

    EXPECT_EQ(1, counter.countExcludedStrings(5, 2));
}

TEST(word_counter, with_1_letter_of_length_5_except_a_aa) {
    StringsCounter counter;
    counter.init({"a", "aa"});

    EXPECT_EQ(0, counter.countExcludedStrings(5, 1));
}

TEST(word_counter, with_2_letters_of_length_5_except_ab) {
    StringsCounter counter;
    counter.init({"ab"});

    EXPECT_EQ(6, counter.countExcludedStrings(5, 2));
}

TEST(word_counter, with_2_letters_of_length_5) {
    StringsCounter counter;
    counter.init({});

    EXPECT_EQ(32, counter.countExcludedStrings(5, 2));
}
