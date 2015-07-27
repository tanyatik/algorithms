#include <vector>

#include <gtest/gtest.h>

#include "sort/sort.hpp"
#include "sort/order_statistics.hpp"
#include "test_helper.hpp"

using namespace algorithms;

TEST(sorted_insert, size_one) {
    std::vector<int> init_array = { 2 };

    auto array = init_array;
    SortedInsert(1, &array);
    TestVector({1, 2}, array);

    array = init_array;
    SortedInsert(3, &array);
    TestVector({2, 3}, array);
}

TEST(sorted_insert, size_two) {
    std::vector<int> init_array = { 2, 4 };

    auto array = init_array;
    SortedInsert(1, &array);
    TestVector({1, 2, 4}, array);

    array = init_array;
    SortedInsert(3, &array);
    TestVector({2, 3, 4}, array);

    array = init_array;
    SortedInsert(5, &array);
    TestVector({2, 4, 5}, array);
}

TEST(sorted_insert, random) {
    const int MIN_ELEMENT = -1000;
    const int MAX_ELEMENT = 1000;
    const int MAX_ITERATIONS = 10;

    auto generator = std::default_random_engine(std::time(nullptr));
    auto init_vector = InitRandomVector(&generator, MIN_ELEMENT, MAX_ELEMENT);

    for (int iteration = 0; iteration < MAX_ITERATIONS; ++iteration) {
        auto vector = init_vector;
        auto expected_vector = init_vector;

        auto element = std::normal_distribution<int>(MIN_ELEMENT, MAX_ELEMENT)(generator);
        expected_vector.insert(expected_vector.begin(), element);
        std::sort(expected_vector.begin(), expected_vector.end());

        SortedInsert(element, &vector);

        TestVector(expected_vector, vector);
    }
}

TEST(merge_sequences, one_element) {
    std::vector<int> expected_vector = {1, 2};
    auto merged_vector = MergeSequences({{1}, {2}});
    TestVector(expected_vector, merged_vector);
}

TEST(merge_sequences, equal_elements) {
    std::vector<int> expected_vector = {1, 1, 2, 2};
    auto merged_vector = MergeSequences({{1, 2}, {1, 2}});
    TestVector(expected_vector, merged_vector);
}

TEST(merge_sequences, two_elements) {
    std::vector<int> expected_vector = {1, 2, 3, 4};
    auto merged_vector = MergeSequences({{1, 3}, {2, 4}});
    TestVector(expected_vector, merged_vector);
expected_vector = {1, 2, 2, 3};
    merged_vector = MergeSequences({{1, 3}, {2, 2}});
    TestVector(expected_vector, merged_vector);
}

TEST(merge_sequences, many_elements) {
    std::vector<int> expected_vector = {1, 2, 3, 4, 5, 6, 7, 8};
    auto merged_vector = MergeSequences({{1, 4}, {2, 8}, {3, 7}, {5, 6}});
    TestVector(expected_vector, merged_vector);

    expected_vector = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    merged_vector = MergeSequences({{1, 3, 5, 7, 9}, {2, 4, 6, 8, 10}});
    TestVector(expected_vector, merged_vector);
}


TEST(order_statistics, stress) {
    const int ITERATIONS = 100;
    const int LENGTH = 10;
    const int MIN = -10;
    const int MAX = 10;

    for (int iteration = 0; iteration < ITERATIONS; ++iteration) {
        std::default_random_engine generator;

        std::uniform_int_distribution<int> distribution(MIN, MAX);

        std::vector<int> sequence;
        for (int index = 0; index < LENGTH; ++index) {
            sequence.push_back(distribution(generator));
        }

        std::vector<int> sorted_sequence = sequence;
        std::sort(sorted_sequence.begin(), sorted_sequence.end());
        for (int order = 0; order < LENGTH; ++order) {
            EXPECT_EQ(sorted_sequence[order], OrderStatistics(sequence, order));
        }
    }
}


void TestPartition(std::vector<int> partitioned, size_t pivot_index) {
    auto pivot_iterator = partitioned.begin() + pivot_index;
    auto partitioned_pivot = Partition(partitioned.begin(), partitioned.end(), pivot_iterator);

    for (auto iterator = partitioned.begin(); iterator != partitioned_pivot; ++iterator) {
        EXPECT_LE(*iterator, *partitioned_pivot);
    }
    for (auto iterator = partitioned_pivot + 1; iterator != partitioned.end(); ++iterator) {
        EXPECT_GT(*iterator, *partitioned_pivot);
    }
}

TEST(partition, simple) {
    TestPartition({6, 5, 4, 3, 2, 1}, 3);
    TestPartition({6, 5, 4, 3, 2, 1}, 0);
    TestPartition({6, 5, 4, 3, 2, 1}, 5);

    TestPartition({1, 2, 3, 4, 5, 6}, 3);
    TestPartition({1, 2, 3, 4, 5, 6}, 0);
    TestPartition({1, 2, 3, 4, 5, 6}, 5);
}

TEST(partition, non_unique) {
    TestPartition({3, 2, 2, 2, 1}, 0);
    TestPartition({3, 2, 2, 2, 1}, 2);
    TestPartition({3, 2, 2, 2, 1}, 3);

    TestPartition({1, 3, 2, 2, 3, 2, 1}, 0);
    TestPartition({1, 3, 2, 2, 3, 2, 1}, 2);
    TestPartition({1, 3, 2, 2, 3, 2, 1}, 3);
    TestPartition({1, 3, 2, 2, 3, 2, 1}, 5);
}

TEST(partition, stress) {
    const int ITERATIONS = 100;
    const int LENGTH = 100;
    const int MIN = -10;
    const int MAX = 10;

    for (int iteration = 0; iteration < ITERATIONS; ++iteration) {
        std::default_random_engine generator;

        std::uniform_int_distribution<int> distribution(MIN, MAX);

        std::vector<int> sequence;
        for (int index = 0; index < LENGTH; ++index) {
            sequence.push_back(distribution(generator));
        }

        for (int index = 0; index < LENGTH; ++index) {
            TestPartition(sequence, index);
        }
    }
}

TEST(order_statistics, simple) {
    EXPECT_EQ(1, OrderStatistics<std::vector<int>>({1, 3, 5, 6, 12, 27}, 0));
    EXPECT_EQ(3, OrderStatistics<std::vector<int>>({1, 3, 5, 6, 12, 27}, 1));
    EXPECT_EQ(5, OrderStatistics<std::vector<int>>({1, 3, 5, 6, 12, 27}, 2));
    EXPECT_EQ(27, OrderStatistics<std::vector<int>>({1, 3, 5, 6, 12, 27}, 5));
}

TEST(order_statistics, non_unique) {
    EXPECT_EQ(2, OrderStatistics<std::vector<int>>({1, 2, 2, 3}, 2));
    EXPECT_EQ(2, OrderStatistics<std::vector<int>>({1, 2, 2, 3}, 1));
    EXPECT_EQ(1, OrderStatistics<std::vector<int>>({1, 2, 2, 3}, 0));
    EXPECT_EQ(3, OrderStatistics<std::vector<int>>({3, 2, 3, 5, 7, 12, 1}, 3));
    EXPECT_EQ(5, OrderStatistics<std::vector<int>>({3, 2, 3, 5, 7, 12, 1}, 4));
}

TEST(non_unique_order_statistics, stress) {
    const int ITERATIONS = 100;
    const int LENGTH = 10;
    const int MIN = -10;
    const int MAX = 10;

    for (int iteration = 0; iteration < ITERATIONS; ++iteration) {
        std::default_random_engine generator;

        std::uniform_int_distribution<int> distribution(MIN, MAX);

        std::vector<int> sequence;
        for (int index = 0; index < LENGTH; ++index) {
            sequence.push_back(distribution(generator));
        }

        std::vector<int> sorted_sequence = sequence;
        std::sort(sorted_sequence.begin(), sorted_sequence.end());
        for (int order = 0; order < sorted_sequence.size(); ++order) {
            EXPECT_EQ(sorted_sequence[order], OrderStatistics(sequence, order))
                << "order " << order;
        }
    }
}

TEST(two_sorted_arrays_median, stress) {
    const int ITERATIONS = 1;
    const int LENGTH = 10000;
    const int MIN = -1000;
    const int MAX = 1000;

    for (int iteration = 0; iteration < ITERATIONS; ++iteration) {
        std::default_random_engine generator;

        std::vector<int> sequence = InitRandomVector(&generator, MIN, MAX, LENGTH);
        std::sort(sequence.begin(), sequence.end());

        double expected_median = FindMedianArray(sequence);

        for (auto separator = sequence.begin(); separator != sequence.end(); ++separator) {
            std::vector<int> sequence1(sequence.begin(), separator);
            std::vector<int> sequence2(separator, sequence.end());

            double got_median = FindMedianSortedArrays(sequence1, sequence2);

            EXPECT_EQ(expected_median, got_median);
        }

        auto empty = std::vector<int>{};
        double got_median = FindMedianSortedArrays(sequence, empty);
        EXPECT_EQ(expected_median, got_median);
    }
}
