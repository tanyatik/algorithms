#include <algorithm>
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

    tree.initPreorderRecursion(keys_preorder);
    tree.debugPrint();
}

TEST(binary_search_tree, preorder_no_recursion) {
    std::vector<int> keys_preorder{5, 2, 1, 3, 6, 7, 9};
    BSTree tree;
    tree.initPreorderNoRecursion(keys_preorder);
    tree.debugPrint();

    keys_preorder = {4, 2, 1, 3, 6, 5, 7};
    BSTree tree2;
    tree2.initPreorderNoRecursion(keys_preorder);
    tree2.debugPrint();

    keys_preorder = {5, 3, 2, 3, 5, 6};
    BSTree tree3;
    tree3.initPreorderNoRecursion(keys_preorder);
    tree3.debugPrint();
}

TEST(binary_search_tree, postorder) {
    std::vector<int> keys_preorder{5, 2, 1, 3, 6, 7, 9};
    BSTree tree;
    tree.initPreorderNoRecursion(keys_preorder);
    tree.debugPrint();

    std::vector<int> postorder = tree.outputPostorderRecursion();
    std::for_each(postorder.begin(), postorder.end(), [](int i) { std::cout << i << ' '; });
    std::cout << std::endl;

    std::vector<int> postorder2 = tree.outputPostorderNoRecursion();
    std::for_each(postorder2.begin(), postorder2.end(), [](int i) { std::cout << i << ' '; });
    std::cout << std::endl;
}
