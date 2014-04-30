#include <gtest/gtest.h>

template<typename TElement>
inline void testVector(std::vector<TElement> expected, std::vector<TElement> result) {
    ASSERT_EQ(expected.size(), result.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], result[i]);
    }
}

