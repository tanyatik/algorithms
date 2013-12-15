#include <vector>
#include <functional>
#include <random>
#include <iostream>

class UniversalHashFunctor {
    public:
        UniversalHashFunctor(int a_parameter, int b_parameter, int prime);
        UniversalHashFunctor();
        // result = ( ( a_parameter_ * number + b_parameter_ ) % prime_
        int operator () (int number) const; 

    private:
        unsigned long long a_parameter_;
        unsigned long long b_parameter_;
        unsigned long long prime_;
};

// Static perfect hash table 
class PerfectHashTable {
    public:
        bool contains(int element) const;
        void initialize(const std::vector<int> &elements);
    private:
        static const int PRIME_NUMBER = 2000000011;
        // Computes index of an element
        // by calculating hash function value by modulus elements_.size()
        int getIndex(int element) const;

        UniversalHashFunctor function_;
        std::vector<int> elements_;
};

class FixedSet {
    public:
        void initialize(const std::vector<int>& elements);
        bool contains(int element) const;

    private:
        static const int PRIME_NUMBER = 2000000011;
        static const int ADJUST = 1000000000;

        UniversalHashFunctor function_;
        std::vector<PerfectHashTable> buckets_;

        // Adds fixed constant ADJUST to every element
        // (it is needed to be able to keep 'negative' numbers in hash table)
        static int adjustElement(int element);

        // Computes index in buckets_ array of an element
        // by calculating hash function value by modulus buckets_.size()
        int getIndex(int element) const;
};

UniversalHashFunctor generateUniversalHashFunctor(int prime);

std::vector<int> readInputVector();
void outputAnswers(const std::vector<bool> &answers);
std::vector<bool> processQueries(const std::vector<int>& numbers, 
                                 const std::vector<int>& queries);

int main() {
    std::ios_base::sync_with_stdio(false);

    std::vector<int> numbers = readInputVector();
    std::vector<int> queries = readInputVector();

    auto answers = processQueries(numbers, queries);

    outputAnswers(answers);

    return 0;
}

UniversalHashFunctor::UniversalHashFunctor(int a_parameter, 
                                           int b_parameter, 
                                           int prime) :
    a_parameter_(a_parameter),
    b_parameter_(b_parameter),
    prime_(prime) {}

UniversalHashFunctor::UniversalHashFunctor() :
    a_parameter_(0),
    b_parameter_(0),
    prime_(1) {}

int UniversalHashFunctor::operator () (int number) const {
    unsigned long long number_long = number;
    unsigned long long factor = a_parameter_ * number_long + b_parameter_; 

    unsigned long long result = (factor % prime_);
    return static_cast<int>(result);
}


UniversalHashFunctor generateUniversalHashFunctor(int prime) {
    unsigned seed = 237;
    static std::minstd_rand0 generator (seed);

    int a_random = std::uniform_int_distribution<int>(1, prime - 1) (generator);
    int b_random = std::uniform_int_distribution<int>(0, prime - 1) (generator);

    return UniversalHashFunctor(a_random, b_random, prime);
}


void PerfectHashTable::initialize(const std::vector<int>& elements) {
    if (elements.size() == 0) {
        return;

    } else if (elements.size() == 1) {
        elements_.resize(1);
        elements_[0] = elements[0];
        return;
    }

    bool ok_function = false;
    int size = elements.size() * elements.size();
    std::vector<bool> occupied_elements(size, false);

    while (!ok_function) {
        elements_.assign(size, 0);
        function_ = generateUniversalHashFunctor(PRIME_NUMBER);
        ok_function = true;
        occupied_elements.assign(size, false);

        for (auto iterator = elements.begin(); iterator != elements.end(); ++iterator) {
            int result = getIndex(*iterator);
            if (occupied_elements[result]) { // collision detected
                ok_function = false;
                break;
            } else {
                occupied_elements[result] = true;
                elements_[result] = *iterator;
            }
        }
    }
}

bool PerfectHashTable::contains(int element) const {
    if (elements_.empty()) {
        return false;
    }
    int index = getIndex(element);
    return (elements_[index] == element);
}

int PerfectHashTable::getIndex(int element) const {
    return function_(element) % elements_.size();
}

void FixedSet::initialize(const std::vector<int>& elements) {
    int size = elements.size();
    std::vector<std::vector <int> > collisions(size);
    buckets_.resize(size);

    function_ = generateUniversalHashFunctor(PRIME_NUMBER);
    
    for (auto iterator = elements.begin(); iterator != elements.end(); ++iterator) {
        int element = adjustElement(*iterator);
        int index = getIndex(element);
        collisions[index].push_back(element);
    }

    for (int bucket_index = 0; bucket_index != size; ++bucket_index) {
        buckets_[bucket_index].initialize(collisions[bucket_index]);
    }
}

int FixedSet::adjustElement(int element) {
    return element + ADJUST;
}

bool FixedSet::contains(int element) const {
    element = adjustElement(element);
    int index = getIndex(element);
    return buckets_[index].contains(element);
}

int FixedSet::getIndex(int element) const {
    return function_(element) % buckets_.size();
}

std::vector<int> readInputVector() {
    int size;
    std::cin >> size;

    std::vector<int> elements(size);
    for (auto& element : elements) {
        std::cin >> element;
    }

    return elements;
}

std::vector<bool> processQueries(const std::vector<int>& numbers, 
                                 const std::vector<int>& queries) {
    FixedSet set;
    set.initialize(numbers);

    std::vector<bool> answers(queries.size());
    int answer_index = 0;

    for (auto query : queries) {
        answers[answer_index++] = set.contains(query);
    }

    return answers;
}

void outputAnswers(const std::vector<bool>& answers) {
    for (auto answer : answers) {
        std::cout << (answer ? "Yes\n" : "No\n");
    }
}
