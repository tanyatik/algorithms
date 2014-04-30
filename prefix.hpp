#include <iostream>
#include <vector>
#include <string>

namespace tanyatik {

// Computes prefix function for string s
inline std::vector<size_t> computePrefix(const std::string &string) {
    std::vector<size_t> prefix(string.size(), 0);

    prefix[0] = 0;
    size_t current = 0;

    for (size_t index = 0; index + 1 < (size_t) string.size(); ++index) {
        // Fill prefix[i + 1]
        current = prefix[index];
        bool stop = false;

        while (!stop) {
            if (string[index + 1] == string[current]) {
                prefix[index + 1] = current + 1;
                stop = true;
            } else if (current == 0) {
                stop = true;
            } else {
                current = prefix[current - 1];
            }
        }
    }
    
    return std::move(prefix);
}

inline std::vector<size_t> computeZFunction(std::string::const_iterator begin, 
        std::string::const_iterator end) {
    size_t size = std::distance(begin, end);
    
    std::vector<size_t> z_function(size, 0);
    
    z_function[0] = size;
    size_t most_right_match_begin = 0;
    size_t most_right_match_end = 0;

    for (size_t index = 1; index < size; ++index) {
        if (most_right_match_end > index) {
            z_function[index] = std::min(z_function[index - most_right_match_begin], 
                most_right_match_end - index);
        }    

        size_t index_left = z_function[index];
        size_t index_right = index + z_function[index];

        while (index_right < size && *(begin + index_left) == *(begin + index_right)) {
            z_function[index]++;
            index_left++;
            index_right++;
        }

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

/*
inline std::vector<size_t> computeZFunction(const std::string &string) {
    std::vector<size_t> z_function(string.size(), 0);
    
    z_function[0] = string.size();
    size_t most_right_match_begin = 0;
    size_t most_right_match_end = 0;

    for (size_t index = 1; index < string.size(); ++index) {
        if (most_right_match_end > index) {
            z_function[index] = std::min(z_function[index - most_right_match_begin], 
                most_right_match_end - index);
        }    

        size_t index_left = z_function[index];
        size_t index_right = index + z_function[index];

        while (index_right < string.size() && string[index_left] == string[index_right]) {
            z_function[index]++;
            index_left++;
            index_right++;
        }

        if (z_function[index] > 0 && index + z_function[index] > most_right_match_end) {
            most_right_match_begin = index;
            most_right_match_end = index + z_function[index];
        }
    }
    
    return std::move(z_function);
}
*/
} // namespace tanyatik
