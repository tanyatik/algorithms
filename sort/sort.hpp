#include <algorithm>
#include <vector>

namespace algorithms {


template<typename TIter, typename F>
TIter MedianPivot(TIter begin, TIter end, F comp) {
    int idx = (static_cast<int>(end - begin)) >> 1;
    TIter middle = begin + idx;

    if (comp(*begin, *end)) {
        if (comp(*end, *middle)) {
            return end;
        } else if (comp(*begin, *middle)) {
            return middle;
        } else {
            return begin;
        }
    } else if (comp(*middle, *end)) {
        return end;
    } else if (comp(*begin, *middle)) {
        return begin;
    } else {
        return middle;
    }
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
void Sort(TIterator begin,
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


class MergeIterator {
private:
    std::vector<std::vector<int>>::const_iterator sequence_iter_;
    std::vector<int>::const_iterator element_iter_;

public:
    MergeIterator(std::vector<std::vector<int>>::const_iterator sequence_iter,
            std::vector<int>::const_iterator element_iter) :
        sequence_iter_(sequence_iter),
        element_iter_(element_iter)
        {}

    MergeIterator(const MergeIterator &other) = default;

    bool stop() const {
        return element_iter_ == sequence_iter_->end();
    }

    MergeIterator CreateNext() const {
        return MergeIterator(sequence_iter_, element_iter_ + 1);
    }

    bool operator > (const MergeIterator &other) const {
        return (*element_iter_ > *other.element_iter_);
    }

    bool operator < (const MergeIterator &other) const {
        return (*element_iter_ < *other.element_iter_);
    }

    int operator * () const {
        return *element_iter_;
    }
};


std::vector<int> MergeSequences(const std::vector<std::vector<int>> &sequences) {
    std::vector<MergeIterator> merge_iterators_heap;
    std::vector<int> merge;

    for (auto sequence_it = sequences.begin(); sequence_it != sequences.end(); ++sequence_it) {
        merge_iterators_heap.push_back(MergeIterator(sequence_it, sequence_it->begin()));
        std::push_heap(merge_iterators_heap.begin(), merge_iterators_heap.end(), std::greater<MergeIterator>());
    }

    while (merge_iterators_heap.size() > 0) {
        MergeIterator minimum = merge_iterators_heap.front();
        std::pop_heap(merge_iterators_heap.begin(), merge_iterators_heap.end(), std::greater<MergeIterator>());
        merge_iterators_heap.pop_back();

        merge.push_back(*minimum);

        auto next_merge_iterator = minimum.CreateNext();
        if (!next_merge_iterator.stop()) {
            merge_iterators_heap.push_back(next_merge_iterator);
            std::push_heap(merge_iterators_heap.begin(), merge_iterators_heap.end(), std::greater<MergeIterator>());
        }
    }

    return merge;
}

template<typename T, typename C>
typename C::iterator BinarySearch(T element, C *container) {
    if (container->empty()) {
        return container->begin();
    }

    auto begin = container->begin();
    auto end = container->end() - 1;
    while (begin != end) {
       int distance = std::distance(begin, end) / 2;
       auto middle = begin + distance;
       if (element < *middle) {
            end = middle;
       } else {
            begin = middle + 1;
       }
    }

    auto insert_position = begin;
    if (*insert_position < element) {
        insert_position = container->end();
    }

    return insert_position;
}

template<typename T, typename C>
void SortedInsert(T element, C *container) {
    auto insert_position = BinarySearch(element, container);
    container->insert(insert_position, element);
}

template<typename TIter, typename F>
void QuickSort(TIter input_begin, TIter input_end, F comp) {
    TIter lhs = input_begin, rhs = input_end - 1;
    if (lhs >= rhs) return;
    if (lhs + 1 == rhs) {
        if (comp(*rhs, *lhs)) std::swap(*lhs, *rhs);
        return;
    }
    TIter pivot = MedianPivot(lhs, rhs - 1, comp);
    TIter store_index = lhs;
    auto pivot_value = *pivot;
    std::swap(*pivot, *rhs);

    for (TIter it = lhs; it != rhs; ++it) {
        if (!comp(pivot_value, *it)) {
            std::swap(*it, *store_index);
            ++store_index;
        }
    }
    std::swap(*store_index, *rhs);

    QuickSort(input_begin, store_index, comp);
    QuickSort(store_index + 1, input_end, comp);
    // assert(std::is_sorted(input_begin, input_end));
}

// Given 3 values,
// sorts them in ascending order.
template<typename TValue>
void Sort3(TValue& a, TValue& b, TValue& c) {
    TValue t_a = a;
    TValue t_b = b;
    TValue t_c = c;

    using std::swap;

    if (a <= b && a <= c) {
        if (b <= c) {
            // a, b, c
            a = t_a;
            b = t_b;
            c = t_c;
        } else {
            // a, c, b
            a = t_a;
            b = t_c;
            c = t_b;
        }
    } else if (b <= a && b <= c) {
        if (a <= c) {
            // b, a, c
            a = t_b;
            b = t_a;
            c = t_c;
        } else {
            // b, c, a
            a = t_b;
            b = t_c;
            c = t_a;
        }
    } else {
        if (a <= b) {
            // c, a, b
            a = t_c;
            b = t_a;
            c = t_b;
        } else {
            // c, b, a
            a = t_c;
            b = t_b;
            c = t_a;
        }
    }
}


} // namespace algorithms
