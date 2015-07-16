#include <algorithm>
#include <vector>

namespace algorithms {


template<typename TIter, typename T = typename TIter::value_type, typename C = std::less<T>>
T MedianPivot(TIter begin, TIter end, C comparator = C()) {
    const int ROWS_NUMBER = 5;

    T max_value = std::numeric_limits<T>::max();
    size_t elements_number = std::distance(begin, end);

    if (elements_number <= ROWS_NUMBER) {
        std::vector<T> elements(ROWS_NUMBER, max_value);
        
        TIter iterator = begin;
        for (int index = 0; iterator != end; ++index) {
            elements[index] = *iterator++;
        }

        Sort(elements.begin(), elements.end());
        int median_index = elements_number % 2 == 0 ? elements_number / 2 - 1 : elements_number / 2;

        return elements[median_index];
    }

    int columns_number = elements_number / ROWS_NUMBER;
    if (elements_number % ROWS_NUMBER != 0) {
        ++columns_number;
    }
    int table_size = columns_number * ROWS_NUMBER;
    assert(table_size % ROWS_NUMBER == 0);

    int column_index = 0;
    int row_index = 0;
    std::vector<std::vector<T>> table(columns_number, std::vector<T>(ROWS_NUMBER, max_value));
    for (TIter iterator = begin; iterator != end; ++iterator) {
        table[column_index][row_index++] = *iterator;
        if (row_index == ROWS_NUMBER) {
            row_index = 0;
            ++column_index;
        }
    }

    for (auto &column : table) {
        Sort(column.begin(), column.end());
r    }`
    std::vector<T> middle_row;
    for (auto &column : table) {
        middle_row.push_back(column[ROWS_NUMBER / 2]);
    }

    return MedianPivot(middle_row.begin(), middle_row.end(), comparator);
}

// Merges two parts of an input container into output container
// 'begin_iter' should point to the beginning of the first part to merge
// 'middle_iter' should point to the beginning of the second part to merge
// 'end_iter' should point to the past-to-the-end of the second part to merge
// 'begin_output' should point to the beggining of the container,
// which size is enough to place all element of merged containers
template<typename TInputIterator, typename TOutputIterator, typename TComparator>
void Merge(TInputIterator begin_iter, 
        TInputIterator middle_iter, 
        TInputIterator end_iter, 
        TOutputIterator begin_output, 
        TOutputIterator end_output,
        TComparator comparator) {
    TInputIterator begin_first = begin_iter, begin_second = middle_iter;
 
    // While there are elements in the left or right runs
    for (TInputIterator j = begin_output; j < end_output; ++j) {
        // If left run head exists and is <= existing right run head.
        if (begin_first < middle_iter && 
                (begin_second >= end_iter || !comparator(*begin_second, *begin_first)))
            *j = *begin_first++;  // Increment begin_first after using it as an index.
        else
            *j = *begin_second++;  // Increment begin_second after using it as an index.
    }
}

// Sorts an container according to comparator
// (which is assuming to return true if first argument is less than second)
// 'input_begin' should point to the beggining of the container,
// 'input_end' should point to the past-to-the-end element
// 'temp_begin' and 'temp_end' point to begin and end
// of the temporary container which is needed to merge
// Temporary container should be of the same size than input container
template<typename TInputIterator, typename TOutputIterator, typename TComparator>
void MergeSort(TInputIterator input_begin, 
        TInputIterator input_end, 
        TOutputIterator temp_begin, 
        TOutputIterator temp_end, 
        TComparator comp) {
    if (input_end <= input_begin + 1) {
        return;
    }
    size_t middle_idx = std::distance(input_begin, input_end) >> 1;
    TInputIterator middle = input_begin + middle_idx;
    TOutputIterator temp_middle = temp_begin + middle_idx;

    MergeSort(input_begin, middle, temp_begin, temp_middle, comp);
    MergeSort(middle, input_end, temp_middle, temp_end, comp);
    Merge(input_begin, middle, input_end, temp_begin, temp_end, comp);

    std::copy(temp_begin, temp_end, input_begin); 
}

// Sorts an container according to comparator
// 'comp' is a comparator functions that returns true if first argument is less than second,
// 'input_begin' should point to the beggining of the container,
// 'input_end' should point to the past-to-the-end element
template<typename TIterator, typename TComparator = std::less<typename TIterator::value_type>>
void MergeSort(TIterator begin, 
        TIterator end, 
        TComparator comparator = TComparator()) {
    auto temp_container = std::vector<typename TIterator::value_type>(begin, end);
    MergeSort(begin, end, temp_container.begin(), temp_container.end(), comparator);
}


// Given sequence [begin, end],
// splits it into two sequences *left_part, *right_part
// such that every element in *left_part is <= pivot,
// every element in *right_part is > pivot
template<typename TIter, typename V, typename T = typename TIter::value_type, typename C = std::less<T>>
void Partition(TIter begin, TIter end, T pivot, V *left_part, V *right_part, C comparator = C()) {
    for (TIter iterator = begin; iterator != end; ++iterator) {
        if (comparator(*iterator, pivot)) {
            left_part->push_back(*iterator);
        } else {
            right_part->push_back(*iterator);
        }
   }
}

// Given sequence [begin, end),
// returns element that is k-th in sorted sequence [begin, end)
template<typename TIter, typename T = typename TIter::value_type, typename C = std::less<T>>
T OrderStatistics(TIter begin, TIter end, int order, C comparator = C()) {
    std::cerr << "input ";
    for (auto iter = begin; iter != end; ++iter) {
        std::cerr << *iter << " ";
    }
    T pivot = MedianPivot(begin, end);
    std::cerr << "order " << order << " chosen pivot " << pivot << std::endl;

    std::vector<T> left_part;
    std::vector<T> right_part;

    Partition(begin, end, pivot, &left_part, &right_part, comparator); 
    assert(left_part.size() < std::distance(begin, end));
    if (right_part.size() == std::distance(begin, end)) {
        // all elements are equal?
        T first_element = *begin;
        for (TIter iterator = begin + 1; iterator != end; ++iterator) {
            assert(*iterator == first_element);
        }

        return first_element;
    }

    if (left_part.size() >= order) {
        return OrderStatistics(left_part.begin(), left_part.end(), order, comparator);
    } else {
        return OrderStatistics(right_part.begin(), 
                right_part.end(), 
                order - left_part.size(), 
                comparator);
    }
}

template<typename V, typename C = std::less<typename V::value_type>>
typename V::value_type OrderStatistics(const V &container, int order, C comparator = C()) {
    return OrderStatistics(container.begin(), container.end(), order, comparator);
}

} // namespace algorithms
