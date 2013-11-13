#include <vector>
#include <functional>
#include <random>
#include <chrono>
#include <algorithm>

//#define PRINT_STATISTICS

typedef std::function<int (int)> HashFunction;

int universalHashFunction(int k_param, int a_param, int b_param, int p_param, int m_param) {
    unsigned long long a_l = a_param;
    unsigned long long b_l = b_param;
    unsigned long long k_l = k_param;

    unsigned long long factor = a_l * k_l + b_l; 
    
    unsigned long long result = (factor % p_param);
    result = (result % m_param);
    return (int) result;
}

HashFunction pickupUniversalHashFunction(int p_param, int m_param) {
    using namespace std::placeholders;

    static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::minstd_rand0 generator (seed);

    int a_random = std::uniform_int_distribution<int>(1, p_param - 1) (generator);
    int b_random = std::uniform_int_distribution<int>(0, p_param - 1) (generator);

    return std::bind(universalHashFunction, _1, a_random, b_random, p_param, m_param);
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
    assert(elems_size > 0);
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

#ifdef PRINT_STATISTICS
    int counter = 0;
#endif // PRINT_STATISTICS
    int size = chooseSize(elems.size());
    while (!ok_function) {
#ifdef PRINT_STATISTICS
        ++counter;
        if (counter > 1000) {
            std::cout << "Warning: counter > 100000 for " << elems.size() << " elements\n";
            //break;
        }
#endif // PRINT_STATISTICS
        elements_ = std::vector<int>(size, Constants::SENTINEL);
        function_ = pickupUniversalHashFunction(Constants::PRIME_NUMBER, size);

        ok_function = true;
        for (auto iter = elems.begin(); iter != elems.end(); ++iter) {
            int res = function_(*iter);
            assert(res >= 0 && res < size);
            if (elements_[res] != Constants::SENTINEL) {
                ok_function = false;
                break;
            } else {
                elements_[res] = *iter;
            }
        }
    }
#ifdef PRINT_STATISTICS
    std::cout << "Counter: " << counter << " for " << elems.size() << " elements" << std::endl;
#endif // PRINT_STATISTICS
}

bool HashSet::HashTable::contains(int element) const {
    if (elements_.empty()) {
        return false;
    }
    int idx = function_(element);
    assert(idx >= 0 && idx < (int) elements_.size());
    return (elements_[idx] == element);
}


void HashSet::initialize(const std::vector<int>& elems) {
    srand((unsigned) time(NULL));
    int size = elems.size();
    std::vector<std::vector <int> > collisions(size);

    function_ = pickupUniversalHashFunction(Constants::PRIME_NUMBER, size);
    
    for (auto iter = elems.begin(); iter != elems.end(); ++iter) {
        int elem = adjustElement(*iter);
        int res = function_(elem);
        assert(res >= 0 && res < size);
        collisions[res].push_back(elem);
    }

#ifdef PRINT_STATISTICS
    std::for_each(collisions.begin(), collisions.end(), [](std::vector <int> coll) {
        std::cout << '[' << coll.size() << ']' << std::endl;
    });
#endif // PRINT_STATISTICS

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
    assert(res >= 0 && res < (int) hash_tables_.size());
    return hash_tables_[res].contains(element);
}

