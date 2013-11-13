#include <vector>
#include <functional>
#include <random>
#include <iostream>
#include <chrono>

class UniversalHashFunctor {
    public:
        UniversalHashFunctor(int a_param, int b_param, int p_param, int m_param) :
            a_param_(a_param),
            b_param_(b_param),
            p_param_(p_param),
            m_param_(m_param) {}

        UniversalHashFunctor() :
            a_param_(0),
            b_param_(0),
            p_param_(1),
            m_param_(1) {}

        int operator () (int k_param) const {
            unsigned long long k_long = k_param;
            unsigned long long factor = a_param_ * k_long + b_param_; 

            unsigned long long result = (factor % p_param_);
            result = (result % m_param_);
            return static_cast<int>(result);
        }

    private:
        unsigned long long a_param_;
        unsigned long long b_param_;
        unsigned long long p_param_;
        unsigned long long m_param_;
};


static UniversalHashFunctor pickupUniversalHashFunctor(int p_param, int m_param) {
    static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::minstd_rand0 generator (seed);

    int a_random = std::uniform_int_distribution<int>(1, p_param - 1) (generator);
    int b_random = std::uniform_int_distribution<int>(0, p_param - 1) (generator);

    return UniversalHashFunctor(a_random, b_random, p_param, m_param);
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
                bool contains(int element) const;
                void initialize(const std::vector<int> &elements);
            private:
                int chooseSize(int size) const;
                int getSentinel() const;

                UniversalHashFunctor function_;
                std::vector<int> elements_;
        };
        UniversalHashFunctor function_;
        std::vector<HashTable> hash_tables_;
        static int adjustElement(int element);
};

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
        elements_.assign(size, Constants::SENTINEL);
        function_ = pickupUniversalHashFunctor(Constants::PRIME_NUMBER, size);

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

    function_ = pickupUniversalHashFunctor(Constants::PRIME_NUMBER, size);
    
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
    std::vector<int> queries(size);
    for (int i = 0; i < size; ++i) {
        std::cin >> queries[i];
    }
    for (auto query = queries.begin(); query != queries.end(); ++query) {
        if (set.contains(*query)) {
            std::cout << "Yes\n";
        } else {
            std::cout << "No\n";
        }
    }
    return 0;
}
