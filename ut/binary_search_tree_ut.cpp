#include <gtest/gtest.h>

#include "../binary_search_tree.hpp"

template<>
struct TraitsSentinel<int> {
    static int getMinSentinel() { return -100000000; }
    static int getMaxSentinel() { return  100000000; }
};

typedef BinarySearchTree<int> BSTree;

TEST(binary_search_tree, preorder) {
    std::vector<int> keys_preorder{5, 2, 1, 3, 6, 7, 9};
    BSTree tree;

    tree.initPreorder(keys_preorder);
    tree.debugPrint();
}
