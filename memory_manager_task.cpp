#include <vector>
#include <iostream>
#include <algorithm>
#include <assert.h>

#include "memory_manager.hpp"

std::vector<long long> readInput(unsigned int *number_cells) {
    int size;
    std::cin >> *number_cells >> size;

    std::vector<long long> elements(size);
    for (auto& element : elements) {
        std::cin >> element;
    }

    return elements;
}

std::vector<int> processQueries(int number_cells,
                                const std::vector<long long>& queries) {
    MemoryManager manager(number_cells, queries.size());
    std::vector<int> answers;

    int query_result;

    for (auto query : queries) {
        if (query > 0) {
            query_result = manager.allocate((unsigned int) query);
            if (query_result != -1) {
                ++query_result;
            }
            answers.push_back(query_result);
        } else {
            query = -query - 1;
            manager.deallocate(query);
        }
    }

    return answers;
}

void outputAnswers(const std::vector<int>& answers) {
    for (auto answer : answers) {
        std::cout << answer << std::endl;
    }
}


int main() {
    std::ios_base::sync_with_stdio(false);
    unsigned int memory_cells;

    auto queries = readInput(&memory_cells);
    
    auto answers = processQueries(memory_cells, queries);

    outputAnswers(answers);

    return 0;
}
