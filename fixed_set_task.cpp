#include <vector>
#include <functional>
#include <random>
#include <iostream>
#include <chrono>

typedef int TParam;
typedef std::function <int (TParam) > HashFunction;

int universalHashFunction(int k_param, int a_param, int b_param, int p_param, int m_param) {
    unsigned long long a_l = a_param;
    unsigned long long b_l = b_param;
    unsigned long long k_l = k_param;

    unsigned long long factor = a_l * k_l + b_l; 
    
    unsigned long long result = (factor % p_param);
    result = (result % m_param);
    return static_cast<int>(result);
}

HashFunction pickupUniversalHashFunction(int p_param, int m_param) {
    static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::minstd_rand0 generator (seed);

    int a_random = std::uniform_int_distribution<int>(1, p_param - 1) (generator);
    int b_random = std::uniform_int_distribution<int>(0, p_param - 1) (generator);

    return std::bind(universalHashFunction, 
        std::placeholders::_1, a_random, b_random, p_param, m_param);
}


class HashSet {
    public:
        void initialize(const std::vector<int> &elements);
        bool contains(int element) const;

    private:
        enum Constants {
            PRIME_NUMBER = 2000000011,
            ADJUST = 1000000000,
            SENTINEL = -1000000001,
        };
        class HashTable {
            public:
                HashTable();
                bool contains(int element) const;
                void initialize(const std::vector<int> &elements);
            private:
                int chooseSize(int size) const;
                int getSentinel() const;

                HashFunction function_;
                std::vector<int> elements_;
        };
        HashFunction function_;
        std::vector<HashTable> hash_tables_;
        static int adjustElement(int element);
};

HashSet::HashTable::HashTable() : 
    function_(std::bind(universalHashFunction, std::placeholders::_1, 0, 0, 1, 1)) {}

int HashSet::HashTable::chooseSize(int elems_size) const {
    return elems_size * elems_size;
}

void HashSet::HashTable::initialize(const std::vector<int>& elems) {
    if (elems.size() == 0) {
        return;
    } else if (elems.size() == 1) {
        elements_.resize(1);
        elements_[0] = elems[0];
        return;
    }
    bool ok_function = false;

    int size = chooseSize(elems.size());
    while (!ok_function) {
        elements_ = std::vector<int>(size, Constants::SENTINEL);
        function_ = pickupUniversalHashFunction(Constants::PRIME_NUMBER, size);

        ok_function = true;
        for (auto iter = elems.begin(); iter != elems.end(); ++iter) {
            int res = function_(*iter);
            if (elements_[res] != Constants::SENTINEL) {
                ok_function = false;
                break;
            } else {
                elements_[res] = *iter;
            }
        }
    }
}

bool HashSet::HashTable::contains(int element) const {
    if (elements_.empty()) {
        return false;
    }
    int idx = function_(element);
    return (elements_[idx] == element);
}


void HashSet::initialize(const std::vector<int>& elems) {
    int size = elems.size();
    std::vector<std::vector <int> > collisions(size);

    function_ = pickupUniversalHashFunction(Constants::PRIME_NUMBER, size);
    
    for (auto iter = elems.begin(); iter != elems.end(); ++iter) {
        int elem = adjustElement(*iter);
        int res = function_(elem);
        collisions[res].push_back(elem);
    }

    hash_tables_.resize(size);
    for (int coll = 0; coll != size; ++coll) {
        hash_tables_[coll].initialize(collisions[coll]);
    }
}

int HashSet::adjustElement(int element) {
    return element + Constants::ADJUST;
}

bool HashSet::contains(int element) const {
    element = adjustElement(element);
    int res = function_(element);
    return hash_tables_[res].contains(element);
}

int main() {
    std::ios_base::sync_with_stdio(false);

    int size;
    std::cin >> size;
    std::vector<int> elements(size);
    for (int i = 0; i < size; ++i) {
        std::cin >> elements[i];
    }

    HashSet set;
    set.initialize(elements);

    std::cin >> size;
    int query;
    for (int i = 0; i < size; ++i) {
        std::cin >> query;
        if (set.contains(query)) {
            std::cout << "Yes\n";
        } else {
            std::cout << "No\n";
        }
    }
    return 0;
}
