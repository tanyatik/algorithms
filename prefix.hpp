#include <iostream>
#include <vector>
#include <string>

namespace tanyatik {

// Computes prefix function for string s
inline std::vector<int> computePrefix(const std::string &string) {
    std::vector<int> prefix(string.size(), 0);

    prefix[0] = 0;
    int current = 0;

    for (int index = 0; index + 1 < (int) string.size(); ++index) {
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

inline std::vector<int> computeZFunction(const std::string &string) {
    std::vector<int> z_function(string.size(), 0);
    
    z_function[0] = string.size();
    int most_right_match_begin = 0; // start of prefix with biggest value of (prefix_start + prefix_length)
    int most_right_match_end = 0;

    for (int index = 1; index < (int) string.size(); ++index) {
        // index belongs to [prefix_start, prefix_start + prefix_length]
        if (most_right_match_end > index) {
            z_function[index] = std::min(z_function[index - most_right_match_begin], 
                most_right_match_end - index + 1);
        }    

        int index_left = z_function[index];
        int index_right = index + index_left;

        while (index_right < (int) string.size() && string[index_left] == string[index_right]) {
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

} // namespace tanyatik
