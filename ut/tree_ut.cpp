#include <algorithm>
#include <gtest/gtest.h>

#include "tree/binary_search_tree.hpp"
#include "test_helper.hpp"

using namespace algorithms;

template<>
struct TraitsSentinel<int> {
    static int GetMinSentinel() { return -1000000000; }
    static int GetMaxSentinel() { return  1000000000; }
};

typedef BinarySearchTree<int> BSTree;

TEST(binary_search_tree, preorder) {
    std::vector<int> keys_preorder{5, 2, 1, 3, 6, 7, 9};
    BSTree tree;

    tree.InitPreorder({5, 6});
}

TEST(binary_search_tree, postorder1) {
    std::vector<int> keys_preorder{5, 2, 1, 3, 6, 7, 9};
    BSTree tree;
    tree.InitPreorderRecursion(keys_preorder);
    std::vector<int> postorder = tree.OutputPostorderRecursion();
    TestVector(postorder, {1, 3, 2, 9, 7, 6, 5});
    std::vector<int> postorder2 = tree.OutputPostorder();
    TestVector(postorder2, {1, 3, 2, 9, 7, 6, 5});
}

TEST(binary_search_tree, postorder2) {
    BSTree tree = BSTree();
    tree.InitPreorderRecursion({4, 2, 1, 3, 6, 5, 7});
    std::vector<int> postorder = tree.OutputPostorder();
    TestVector(postorder, {1, 3, 2, 5, 7, 6, 4});
}

TEST(binary_search_tree, postorder3) {
    BSTree tree = BSTree();
    tree.InitPreorderRecursion({5, 3, 2, 3, 5, 6});
    std::vector<int> postorder = tree.OutputPostorder();
    TestVector(postorder, {2, 3, 3, 6, 5, 5});
}

TEST(binary_search_tree, inorder1) {
    std::vector<int> keys_preorder{5, 2, 1, 3, 6, 7, 9};
    BSTree tree;
    tree.InitPreorderRecursion(keys_preorder);
    std::vector<int> inorder = tree.OutputInorder();
}

TEST(binary_search_tree, inorder2) {
    BSTree tree = BSTree();
    tree.InitPreorderRecursion({4, 2, 1, 3, 6, 5, 7});
    std::vector<int> inorder = tree.OutputInorder();
    TestVector(inorder, {1, 2, 3, 4, 5, 6, 7});
}

TEST(binary_search_tree, inorder3) {
    BSTree tree = BSTree();
    tree.InitPreorder({5, 3, 2, 3, 5, 6});
    std::vector<int> inorder = tree.OutputInorder();
    TestVector({2, 3, 3, 5, 5, 6}, inorder);
}

TEST(binary_search_tree, inorder_postorder) {
    BSTree tree;
    tree.InitPreorder({10, 4, 2, 4, 10});
    std::vector<int> postorder = tree.OutputPostorder();
    TestVector({2, 4, 4, 10, 10}, postorder);
    std::vector<int> inorder = tree.OutputInorder();
    TestVector({2, 4, 4, 10, 10}, inorder);
}

