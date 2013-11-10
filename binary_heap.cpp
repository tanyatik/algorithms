#include <vector>
#include <assert.h>
#include <iostream>

#include "binary_heap.hpp"

template<>
class TraitsSentinel<int> {
    public:
        enum {
            MINUS_SENTINEL = -10000000
        };
};

void test() {
    BinaryHeap<int, long long> f(13);

    f.insert(20, 1);
    f.insert(15, 2);
    f.insert(11, 3);
    f.insert(6, 4);
    f.insert(9, 5);
    f.insert(1, 6);
    f.insert(3, 7);
    f.insert(5, 8);
    f.insert(17, 9);
    f.insert(7, 10);
    f.insert(8, 11);
    f.debugPrint();
    std::cout << f.extractMax() << std::endl;
    f.debugPrint();
    std::cout << f.extractMax() << std::endl;
    f.debugPrint();
    std::cout << f.extractMax() << std::endl;
    f.debugPrint();
    std::cout << f.extractMax() << std::endl;
    f.debugPrint();
    std::cout << f.extractMax() << std::endl;
    f.debugPrint();
    std::cout << f.extractMax() << std::endl;
    f.debugPrint();
    std::cout << f.extractMax() << std::endl;
    f.debugPrint();
    std::cout << f.extractMax() << std::endl;
    f.debugPrint();
    std::cout << f.extractMax() << std::endl;
    f.debugPrint();
    std::cout << f.extractMax() << std::endl;
    f.debugPrint();
}

int main() {
    BinaryHeap<int, long long> f(8);
    test();

//    std::cout << "Before all insertions\n";
//    f.debugPrint();
//
//    f.insert(10, 1000);
//    std::cout << "After 1 insertions\n";
//    f.debugPrint();
//
//    f.insert(20, 2000);
//    std::cout << "After 2 insertion\n";
//    f.debugPrint();
//
//    f.insert(30, 3000);
//    std::cout << "After 3 insertion\n";
//    f.debugPrint();
//
//    f.insert(15, 15000);
//    std::cout << "After 4 insertion\n";
//    f.debugPrint();
//
//    f.insert(17, 17000);
//    std::cout << "After 5 insertion\n";
//    f.debugPrint();
//
//    f.insert(3, 3000);
//    std::cout << "After 6 insertion\n";
//    f.debugPrint();
//
//    f.insert(7, 7000);
//    std::cout << "After 7 insertion\n";
//    f.debugPrint();
//
//    f.replace(6, 500, 500000);
//    std::cout << "After replacement\n";
//    f.debugPrint();

    return 0;
}
