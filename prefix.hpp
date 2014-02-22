#include <iostream>
#include <vector>
#include <string>

namespace detail {

inline int computeZFunctionDummy(const std::string string,
            int index_left,
            int index_right) {

    int z_function = 0;
    while(index_right < string.size() && string[index_left] == string[index_right]) {
        z_function++;
        index_left++;
        index_right++;
    }

    return z_function;
}

inline void updateZFunctionLongest(const std::vector<int> z_function, 
            int index,
            int *longest_begin, 
            int *longest_end) {
    if (z_function[index] > 0 && index + z_function[index] > *longest_end) {
        *longest_begin = index;
        *longest_end = z_function[index];
    }
}

} // namespace detail

namespace tanyatik {

// Computes prefix function for string s
inline std::vector<int> computePrefix(const std::string &string) {
    std::vector<int> prefix(string.size(), 0);

    prefix[0] = 0;
    int current = 0;

    for (int index = 0; index + 1 < string.size(); ++index) {
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
    using detail::computeZFunctionDummy;
    using detail::updateZFunctionLongest;

    std::vector<int> z_function(string.size(), 0);
    
    z_function[0] = string.size();
    int longest_begin = 0; // start of prefix with biggest value of (prefix_start + prefix_length)
    int longest_end = 0;

    for (int index = 1; index < string.size(); ++index) {
        // index belongs to [prefix_start, prefix_start + prefix_length]
        if (longest_end > index) {
            // string[index_mirror] = string[index] in the beginning of string
            int index_mirror = index - longest_begin;

            int length = z_function[longest_begin] - index_mirror; 
            
            if (z_function[index_mirror] < length) {
                // can use previous value
                z_function[index] = z_function[index_mirror];
            } else {
                // should count
                z_function[index] = length;

                z_function[index] = length + 
                    computeZFunctionDummy(string, index_mirror + length, index + length);
                updateZFunctionLongest(z_function, index, &longest_begin, &longest_end);
            }       
        } else {
            z_function[index] = computeZFunctionDummy(string, 0, index);
            updateZFunctionLongest(z_function, index, &longest_begin, &longest_end);
        }
    }

    return std::move(z_function);
}

} // namespace tanyatik
