#include <iostream>
#include <array>
#include <functional>
#include <vector>
#include <algorithm>

const int MODULUS = 123456789;

unsigned long long bstCountInner(int beginning, 
                                 int end,
                                 const std::vector<long long>& array,
                                 std::vector<std::vector<unsigned long long> > *answers);

unsigned long long bstCount(const std::vector<long long> &input) {
    std::vector<std::vector<unsigned long long>> answers
        (512, (std::vector<unsigned long long>(512)));
    return bstCountInner(0, input.size() - 1, input, &answers);
}

unsigned long long bstCountInner(int beginning, 
                                 int end,
                                 const std::vector<long long>& array,
                                 std::vector<std::vector<unsigned long long >> *answers) {
    if (beginning > end) {
        return 0;
    }
    if (beginning == end) {
        (*answers)[beginning][end] = 1;
        return 1;
    }
    if ((*answers)[beginning][end] != 0) {
        return (*answers)[beginning][end];
    }

    unsigned long long sum = 0;
    unsigned long long factor = 0;

    sum += bstCountInner(beginning + 1, end, array, answers);
    for (int i = beginning + 1; i < end; ++i) {
        if (array[i - 1] < array[i]) { // array[i] can be root of the tree
            factor = bstCountInner(beginning, i - 1, array, answers) * 
                     bstCountInner(i + 1, end, array, answers);
            sum += factor % MODULUS;
        }
    }
    if (array[end - 1] < array[end]) {
        sum += bstCountInner(beginning, end - 1, array, answers);
    }
    sum %= MODULUS;

    (*answers)[beginning][end] = sum;
    return sum;
}

int main() {
    size_t size;
    
    std::cin >> size;
    std::vector<long long> input(size);
    for (auto &iter : input) {
        std::cin >> iter;
    }

    std::sort(input.begin(), input.end());
    std::cout << bstCount(input) << std::endl;
    return 0;
}
