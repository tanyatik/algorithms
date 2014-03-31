#include <gtest/gtest.h>

#include "../suffix_automata.hpp"

TEST(suffix_automata, count_different_substrings_ab) {
    SuffixAutomata s;
    s.init("ab");

    EXPECT_EQ(3, s.countDifferentSubstrings());
}

TEST(suffix_automata, count_different_substrings_abcbd) {
    SuffixAutomata s;
    s.init("abcbd");

    EXPECT_EQ(14, s.countDifferentSubstrings());
}
