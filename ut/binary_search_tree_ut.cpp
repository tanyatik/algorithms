#include <algorithm>
#include <gtest/gtest.h>

#include "../binary_search_tree.hpp"

template<>
struct TraitsSentinel<int> {
    static int getMinSentinel() { return -1000000000; }
    static int getMaxSentinel() { return  1000000000; }
};

typedef BinarySearchTree<int> BSTree;

TEST(binary_search_tree, preorder) {
    std::vector<int> keys_preorder{5, 2, 1, 3, 6, 7, 9};
    BSTree tree;

    tree.initPreorder({5, 6});
}

TEST(binary_search_tree, preorder_no_recursion) {
    std::vector<int> keys_preorder{5, 2, 1, 3, 6, 7, 9};
    BSTree tree;
    tree.initPreorder(keys_preorder);

    keys_preorder = {4, 2, 1, 3, 6, 5, 7};
    BSTree tree2;
    tree2.initPreorder(keys_preorder);

    keys_preorder = {5, 3, 2, 3, 5, 6};
    BSTree tree3;
    tree3.initPreorder(keys_preorder);
}

void testVector(std::vector<int> expected, std::vector<int> result) {
    ASSERT_EQ(expected.size(), result.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        ASSERT_EQ(expected[i], result[i]);
    }
}

TEST(binary_search_tree, postorder) {
    std::vector<int> keys_preorder{5, 2, 1, 3, 6, 7, 9};
    BSTree tree;
    tree.initPreorderRecursion(keys_preorder);
    std::vector<int> postorder = tree.outputPostorderRecursion();
    testVector(postorder, {1, 3, 2, 9, 7, 6, 5});
    std::vector<int> postorder2 = tree.outputPostorder();
    testVector(postorder2, {1, 3, 2, 9, 7, 6, 5});

    tree = BSTree();
    tree.initPreorderRecursion({4, 2, 1, 3, 6, 5, 7});
    postorder = tree.outputPostorder();
    testVector(postorder, {1, 3, 2, 5, 7, 6, 4});

    tree = BSTree();
    tree.initPreorderRecursion({5, 3, 2, 3, 5, 6});
    postorder = tree.outputPostorder();
    testVector(postorder, {2, 3, 3, 6, 5, 5});
}

TEST(binary_search_tree, inorder) {
    std::vector<int> keys_preorder{5, 2, 1, 3, 6, 7, 9};
    BSTree tree;
    tree.initPreorderRecursion(keys_preorder);
    std::vector<int> inorder = tree.outputInorder();

    tree = BSTree();
    tree.initPreorderRecursion({4, 2, 1, 3, 6, 5, 7});
    inorder = tree.outputInorder();
    testVector(inorder, {1, 2, 3, 4, 5, 6, 7});

    tree = BSTree();
    tree.initPreorder({5, 3, 2, 3, 5, 6});
    inorder = tree.outputInorder();
    testVector({2, 3, 3, 5, 5, 6}, inorder);
}

TEST(binary_search_tree, inorder_postorder) {
    BSTree tree;
    tree.initPreorder({10, 4, 2, 4, 10});
    std::vector<int> postorder = tree.outputPostorder();
    testVector({2, 4, 4, 10, 10}, postorder);
    std::vector<int> inorder = tree.outputInorder();
    testVector({2, 4, 4, 10, 10}, inorder);
}
