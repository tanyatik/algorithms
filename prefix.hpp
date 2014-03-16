#include <iostream>
#include <vector>
#include <string>

namespace tanyatik {

inline size_t computeZValue(std::string::const_iterator begin, 
        std::string const_iterator end, 
        const std::vector<size_t> previous_z_values, 
        size_t index) {

}

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

    prefix[0] = 0;
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


inline std::vector<size_t> getKMPPatternOccurences(const std::string &pattern, 
        const std::string &text) {
    std::vector<size_t> matches;
    // Knutn-Morris-Pratt algorithm 
    // Invariant:
    // for every index < text_index: all matches of pattern are found
    // and put in matches
    size_t text_index = 0;
    // Prefix function for pattern needs to be precomputed
    auto pattern_prefix = computePrefix(pattern); 

    while (text_index + pattern.size() - 1 < text.size()) {
        size_t pattern_index = 0;
       
        while (pattern_index < pattern.size() && text_index + pattern.size() - 1 < text.size()) {
            // assume pattern[0..pattern_index - 1] is matched
            if (pattern[pattern_index] == text[text_index + pattern_index]) {
                ++pattern_index;
            } else {
                size_t longest_border = pattern_prefix[pattern_index];

                if (longest_border == 0) {
                    // go to next symbol
                    pattern_index = 0;
                    ++text_index;    
                } else {
                    text_index += pattern_index - longest_border + 1;
                    pattern_index = longest_border - 1;
                }
            }
        }
        if (pattern_index == pattern.size()) {
            // match found
            matches.push_back(text_index);
            ++text_index;
        }
    }   
    return std::move(matches);
}


inline std::vector<size_t> getZFunctionOccurences(const std::string &pattern,
        const std::string &text) {
    std::vector<size_t> matches;
    std::string pattern_text = pattern + "$" + text;

    std::vector<size_t> z_function = computeZFunction(pattern_text);

    for (size_t index = pattern.size() + 1; index < pattern_text.size(); ++index) {
        if (z_function[index] == pattern.size()) {
            matches.push_back(index - pattern.size() - 1);
        }
    }
    
    return std::move(matches);
}

inline std::vector<size_t> getPatternOccurences(const std::string &pattern, 
        const std::string &text) {

    std::vector<size_t> matches;
 
    return std::move(getZFunctionOccurences(pattern, text));
}

} // namespace tanyatik
