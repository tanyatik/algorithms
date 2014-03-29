#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <algorithm>

namespace tanyatik {

inline std::vector<size_t> computeZFunction(std::string::const_iterator begin, 
        std::string::const_iterator end) {
    size_t size = std::distance(begin, end);
    
    std::vector<size_t> z_function(size, 0);
    
    z_function[0] = size;

    // beginning and end of block 'most right match',
    // which end is the most right among all blocks
    size_t most_right_match_begin = 0;
    size_t most_right_match_end = 0;

    for (size_t index = 1; index < size; ++index) {
        // get the lower bound of z_function[i]
        if (most_right_match_end > index) {
            // most right match block overlaps with current position,
            // that means z_function[index - most_right_match_begin] has length of small block
            // inside most right match block, that matches block in the beginning of i,
            // so we can skip either z_function[index - most_right_match_begin] symbols 
            // (in the case small block fits into most right match block),
            // or most_right_match_end - index,
            // (in the case small block does not fit into most right match block)
            z_function[index] = std::min(z_function[index - most_right_match_begin], 
                most_right_match_end - index);
        }    

        // skip z_function[index] comparisons
        size_t index_left = z_function[index];
        size_t index_right = index + z_function[index];

        // begin to compare string with it's prefix starting at z_function[index]
        while (index_right < size && *(begin + index_left) == *(begin + index_right)) {
            z_function[index]++;
            index_left++;
            index_right++;
        }

        // update most right match data
        if (z_function[index] > 0 && index + z_function[index] > most_right_match_end) {
            most_right_match_begin = index;
            most_right_match_end = index + z_function[index];
        }
    }
    
    return std::move(z_function);
}

inline std::vector<size_t> computeZFunction(const std::string &string) {
    return computeZFunction(string.begin(), string.end());
}

// Computes prefix function for string s
inline std::vector<size_t> computePrefix(const std::string &string) {
    std::vector<size_t> prefix(string.size(), 0);

    size_t current = 0;

    for (size_t index = 0; index + 1 < (size_t) string.size(); ++index) {
        // Fill prefix[index + 1]
        // Use already computed prefix[index]
        current = prefix[index];
        bool stop = false;

        // iterate through all possible prefixes of string
        while (!stop) {
            // prefix[index] matches, and next symbol matches
            if (string[index + 1] == string[current]) {
                prefix[index + 1] = current + 1;
                stop = true;
            } else if (current == 0) {
                // no matching prefix found
                stop = true;
            } else {
                current = prefix[current - 1];
            }
        }
    }
    
    return std::move(prefix);
}

inline std::vector<size_t> getKMPPatternMatches(const std::string &pattern,
        const std::string &text) {
    std::vector<size_t> matches;

    // Special case of empty pattern (occurs in every position of text)
    if (pattern.length() == 0) {
        for (size_t text_index = 0; text_index < text.length(); ++text_index) {
            matches.push_back(text_index);
        }
        return std::move(matches);
    }

    std::vector<size_t> prefix = computePrefix(pattern);
    // Index of current symbol inside pattern
    size_t pattern_index = 0;
    for (size_t text_index = 0; text_index < text.size(); ++text_index) {
        // Check if we need to shift pattern to right
        // (either if pattern is already matched 
        // or if current symbols of text and pattern do not concur)
        while (pattern_index != 0 && 
                (pattern_index == pattern.size() || 
                 text[text_index] != pattern[pattern_index])) {
            pattern_index = prefix[pattern_index - 1];
        }
        // If current symbols of text and pattern concur, 
        // simply keep looking through pattern
        if (text[text_index] == pattern[pattern_index]) {
            ++pattern_index;
        }
        // Match of pattern is found
        if (pattern_index == pattern.size()) {
            matches.push_back(text_index - pattern.size() + 1);
        }
    }

    return std::move(matches);
}

inline std::vector<size_t> getZPatternMatches(const std::string &pattern, 
        const std::string &text) {
    std::vector<size_t> matches;
    std::string pattern_text = pattern + "$" + text;

    std::vector<size_t> z_function = computeZFunction(pattern_text.begin(), pattern_text.end());

    for (size_t index = pattern.size() + 1; index < pattern_text.size(); ++index) {
        if (z_function[index] == pattern.size()) {
            matches.push_back(index - pattern.size() - 1);
        }
    }

    return std::move(matches);
}

inline std::vector<size_t> getPatternMatches(const std::string &pattern, 
        const std::string &text) {
    return std::move(getKMPPatternMatches(pattern, text));
}

struct Subpattern {
    std::string pattern;
    size_t offset;

    Subpattern(std::string pattern, size_t offset) :
        pattern(pattern),
        offset(offset)
        {}

    bool operator ==(const Subpattern &other) const {
        return pattern == other.pattern && offset == other.offset;
    }
};

inline std::vector<Subpattern> getSubpatterns(const std::string &pattern, char delimiter) {
    std::vector<Subpattern> subpatterns;

    auto iter = pattern.begin(); 
    while (iter != pattern.end()) {
        while (iter != pattern.end() && *iter == delimiter) {
            ++iter;
        }
        auto subpattern_begin = iter;
        while (iter != pattern.end() && *iter != delimiter) {
            ++iter;
        }
        auto subpattern_end = iter;

        if (std::distance(subpattern_begin, subpattern_end) > 0)  
            subpatterns.push_back(Subpattern(std::string(subpattern_begin, subpattern_end), 
                                             std::distance(pattern.begin(), subpattern_begin)));
    }
    
    if (subpatterns.empty() && !pattern.empty()) {
        // only "?" in pattern
        subpatterns.push_back(Subpattern("", 0));
    }

    return subpatterns;
}

inline std::vector<size_t> getSubpatternMatches(const Subpattern &subpattern, 
        const std::string &text) {
    auto matches = getPatternMatches(subpattern.pattern, text);
    std::vector<size_t> subpattern_matches;

    for (auto match : matches) {
        int subpattern_match = match - subpattern.offset;
        if (subpattern_match >= 0) {
            subpattern_matches.push_back(subpattern_match);
        }
    }
    
    return std::move(subpattern_matches);
}

static const size_t MAX_PATTERN_LENGTH = 100000;
static const size_t MAX_INTERSECTION_SIZE = MAX_PATTERN_LENGTH;
/*
inline std::vector<size_t> intersectVectors
        (const std::vector<std::vector<size_t>>& vectors) {
    std::vector<size_t> intersection;


    std::bitset<MAX_SET_SIZE> set;   
    bool first_set = true;

    for (const auto &vector : vectors) {
        std::bitset<MAX_SET_SIZE> sub_set;

        for (auto item : vector) {
            sub_set.set(item);
        }
        
        if (first_set) {
            set = sub_set;
            first_set = false;
        } else {
            set &= sub_set;
        }
    }
   
    for (size_t bit = 0; bit < MAX_SET_SIZE; ++bit) {
        if (set.test(bit)) {
            intersection.push_back(bit);
        }
    }

    return std::move(intersection);
}
*/

inline std::vector<size_t> intersectVectors(const std::vector<std::vector<size_t>>& vectors) {
    std::array<size_t, MAX_INTERSECTION_SIZE> counts;
    std::fill(counts.begin(), counts.end(), 0);

    for (const auto &vector : vectors) {
        for (auto vector_item : vector) {
            ++counts[vector_item];
        }
    }

    std::vector<size_t> intersection;
    for (size_t i = 0; i < MAX_INTERSECTION_SIZE; ++i) {
        if (counts[i] == vectors.size()) {
            intersection.push_back(i);
        }
    }

    return std::move(intersection);
}

std::vector<size_t> getFuzzyPatternMatches(const std::string &fuzzy_pattern,
        const std::string &text) {
    const char ANY_SYMBOL = '?';

    std::vector<Subpattern> subpatterns = getSubpatterns(fuzzy_pattern, ANY_SYMBOL);

    std::vector<std::vector<size_t>> subpattern_matches;
    
    for (const auto &subpattern : subpatterns) {
        subpattern_matches.push_back(getSubpatternMatches(subpattern, text));
    }

    std::vector<size_t> fuzzy_pattern_matches = intersectVectors(subpattern_matches);
    auto end = std::remove_if(fuzzy_pattern_matches.begin(), fuzzy_pattern_matches.end(), 
        [&] (size_t match) -> bool {
            return match + fuzzy_pattern.size() > text.size();
        });
    return std::vector<size_t>(fuzzy_pattern_matches.begin(), end);
}

} // namespace tanyatik
