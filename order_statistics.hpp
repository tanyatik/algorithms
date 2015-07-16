#include <algorithm>
#include <vector>

namespace detail {

// Returns position of median in sequence [begin, end)
// Does not modify source sequence
template<typename I, typename T = typename I::value_type, typename C = std::less<T>>
I MedianNaive(I begin, I end, C comparator = C()) {
    std::vector<T> elements(begin, end);

    std::sort(elements.begin(), elements.end());

    int median_index = elements.size() % 2 == 0 ? elements.size() / 2 - 1 : elements.size() / 2;
    for (I iterator = begin; iterator != end; ++iterator) {
        if (*iterator== elements[median_index]) {
            return iterator;
        } 
    }
    assert(!"Unreachable code");
    return begin;
}

// Makes a table of elements in sequence [begin, end) 
// Table has rows_number rows and size / rows_number columns
// Remaining elements are filled woth std::numeric_limits<T>::max
template<typename I, typename T = typename I::value_type>
std::vector<std::vector<T>> FillTable(I begin, I end, size_t rows_number) {
    size_t elements_number = std::distance(begin, end);
    int columns_number = elements_number / rows_number;
    if (elements_number % rows_number != 0) {
        ++columns_number;
    }
    int table_size = columns_number * rows_number;
    assert(table_size % rows_number == 0);

    int column_index = 0;
    int row_index = 0;

    T max_value = std::numeric_limits<T>::max();
    std::vector<std::vector<T>> table(columns_number, std::vector<T>(rows_number, max_value));
    for (I iterator = begin; iterator != end; ++iterator) {
        table[column_index][row_index++] = *iterator;
        if (row_index == rows_number) {
            row_index = 0;
            ++column_index;
        }
    }
    return table;
}    

// Returns position of median of medians in sequence [begin, end)
// Does not modify source sequence
template<typename I, typename T = typename I::value_type, typename C = std::less<T>>
I ApproximateMedianMedianPivot(I begin, I end, C comparator = C()) {
    const int ROWS_NUMBER = 5;

    if (std::distance(begin, end) <= ROWS_NUMBER) {
        return MedianNaive(begin, end, comparator);
    }

    std::vector<std::vector<T>> table = FillTable(begin, end, ROWS_NUMBER);
    for (auto &column : table) {
        std::sort(column.begin(), column.end());
    }

    std::vector<T> middle_row;
    for (auto &column : table) {
        middle_row.push_back(column[ROWS_NUMBER / 2]);
    }

    I median_iterator = ApproximateMedianMedianPivot
        (middle_row.begin(), middle_row.end(), comparator);
    size_t median_index = std::distance(middle_row.begin(), median_iterator);

    return begin + (median_index * ROWS_NUMBER + ROWS_NUMBER / 2);
}

} // namespace detail

// Reorders sequence [begin, end) in such way
// that elements <= pivot are before pivot,
// and elements > pivot are after pivot in sequence 
template<typename I, typename C = std::less<typename I::value_type>> 
I Partition(I begin, I end, I pivot_iterator, C comparator = C()) {
    auto pivot = *pivot_iterator;
    std::swap(*begin, *pivot_iterator);

    I left = begin;
    I right = end - 1;

    assert(left <= right);
    assert(left <= pivot_iterator && pivot_iterator <= right);
    if (left == right) {
        return left;
    }
    while (true) {
        while (comparator(*left, pivot)) {
            ++left;
        }
        while (comparator(pivot, *right)) {
            --right;
        }
        while (*left == *right && left != right) {
            ++left;
        }
        if (left < right) {
            std::swap(*left, *right);
        } else {
            return right;
        }
    }
    assert(!"Unreachable code");
    return right;
}

// Given sequence [begin, end),
// returns element that is k-th in sorted sequence [begin, end)
// k is in [0, |sequence|]
template<typename I, typename T = typename I::value_type, typename C = std::less<T>>
T OrderStatistics(I input_begin, I input_end, int order, C comparator = C()) {
    std::vector<T> sequence(input_begin, input_end);
    auto begin = sequence.begin();
    auto end = sequence.end(); 

    size_t elements_number = std::distance(begin, end); 
    assert(elements_number >= order);

    if (elements_number == 1 && order == 0) {
        return *begin;
    }

    auto pivot_iterator = ApproximateMedianMedianPivot(begin, end, comparator); 
    
    pivot_iterator = Partition(begin, end, pivot_iterator, comparator); 
    size_t pivot_position = std::distance(begin, pivot_iterator); 

    if (order == pivot_position) {
        return *pivot_iterator;
    } else if (order < pivot_position) {
        return OrderStatistics(begin, pivot_iterator, order, comparator);
    } else {
        return OrderStatistics(pivot_iterator + 1, end, order - pivot_position - 1, comparator);
    }
}

// Order is indexed in [0, V.size()) 
template<typename V, typename C = std::less<typename V::value_type>>
typename V::value_type OrderStatistics(const V &container, 
        int order, 
        C comparator = C()) {
    return OrderStatistics(container.begin(), container.end(), order, comparator);
}

