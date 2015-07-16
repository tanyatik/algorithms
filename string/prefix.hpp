#include <iostream>
#include <vector>
#include <string>

namespace algorithms {

// Computes prefix function for string s
std::vector<size_t> ComputePrefix(const std::string &string);

std::vector<size_t> ComputeZFunction(std::string::const_iterator begin,
        std::string::const_iterator end);

std::vector<size_t> ComputeZFunction(const std::string &string);

} // namespace algorithms
