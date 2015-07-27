#pragma once

#include <vector>
#include <random>

namespace algorithms {

std::vector<int> InitRandomVector(std::default_random_engine *generator,
        int min_element,
        int max_element,
        int max_array_size = 10) {
    std::vector<int> vector;
    for (int i = 0; i < max_array_size; ++i) {
        vector.push_back(std::normal_distribution<int>(min_element, max_element)(*generator));
    }

    return vector;
}

template<typename T>
std::vector<std::vector<T> > GenerateData(size_t threads_count, size_t elements_count) {
    std::vector<std::vector<T> > data(threads_count, std::vector<T> (elements_count));
    std::default_random_engine generator;

    for (auto &thread_data: data) {
        for (auto &data_element: thread_data) {
            data_element = std::uniform_int_distribution<T>(0, 100)(generator);
        }
    }

    return std::move(data);
}

template<typename T>
inline void TestVector(std::vector<T> expected, std::vector<T> result) {
    ASSERT_EQ(expected.size(), result.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], result[i]);
    }
}

} // namespace algorithms

