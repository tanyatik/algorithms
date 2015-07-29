#include <algorithm>
#include <gtest/gtest.h>

#include "tree/binary_search_tree.hpp"
#include "tree/splay_tree.hpp"
#include "test_helper.hpp"

using namespace algorithms;

template<>
struct TraitsSentinel<int> {
    static int GetMinSentinel() { return -1000000000; }
    static int GetMaxSentinel() { return  1000000000; }
};

typedef BinarySearchTree<int> BSTree;
typedef SplayTree<int> STree;

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


TEST(binary_search_tree, search) {
    BSTree tree;
    tree.InitPreorder({2, 1, 3});

    ASSERT_EQ(2, tree.Search(2)->GetData());
    ASSERT_EQ(3, tree.Search(3)->GetData());
    ASSERT_EQ(1, tree.Search(1)->GetData());
    ASSERT_EQ(nullptr, tree.Search(4));

    BSTree bigger_tree;
    tree.InitPreorder({10, 4, 2, 4, 10});
    ASSERT_EQ(10, tree.Search(10)->GetData());
    ASSERT_EQ(4, tree.Search(4)->GetData());
    ASSERT_EQ(nullptr, tree.Search(1));
}


TEST(splay_tree, zig_left) {
    STree tree;
    tree.InitPreorder({4, 2, 1, 3, 5});

    tree.Splay(tree.GetRoot()->GetLeft());

    ASSERT_TRUE(tree.IsValid());
    std::vector<int> preorder = tree.OutputPreorder();
    TestVector({2, 1, 4, 3, 5}, preorder);
}

TEST(splay_tree, zig_right) {
    STree tree;
    tree.InitPreorder({2, 1, 4, 3, 5});

    tree.Splay(tree.GetRoot()->GetRight());

    ASSERT_TRUE(tree.IsValid());
    std::vector<int> preorder = tree.OutputPreorder();
    TestVector({4, 2, 1, 3, 5}, preorder);
}

TEST(splay_tree, zig_zig_left) {
    // splaying root
    STree tree;
    tree.InitPreorder({6, 4, 2, 1, 3, 5, 7});

    tree.Splay(tree.GetRoot()->GetLeft()->GetLeft());

    ASSERT_TRUE(tree.IsValid());

    std::vector<int> preorder = tree.OutputPreorder();
    TestVector({2, 1, 4, 3, 6, 5, 7}, preorder);

    // splaying no-root
    STree bigger_tree;
    bigger_tree.InitPreorder({8, 6, 4, 2, 1, 3, 5, 7, 9});

    bigger_tree.Splay(bigger_tree.GetRoot()->GetLeft()->GetLeft()->GetLeft());

    ASSERT_TRUE(bigger_tree.IsValid());

    TestVector({8, 2, 1, 4, 3, 6, 5, 7, 9}, bigger_tree.OutputPreorder());
}

TEST(splay_tree, zig_zig_right) {
    // splaying root
    STree tree;
    tree.InitPreorder({2, 1, 4, 3, 6, 5, 7});

    tree.Splay(tree.GetRoot()->GetRight()->GetRight());

    ASSERT_TRUE(tree.IsValid());
    TestVector({6, 4, 2, 1, 3, 5, 7}, tree.OutputPreorder());


    // splaying no-root
    STree bigger_tree;
    bigger_tree.InitPreorder({8, 2, 1, 4, 3, 6, 5, 7, 9});

    bigger_tree.Splay(bigger_tree.GetRoot()->GetLeft()->GetRight()->GetRight());

    ASSERT_TRUE(bigger_tree.IsValid());

    TestVector({8, 6, 4, 2, 1, 3, 5, 7, 9}, bigger_tree.OutputPreorder());
}


TEST(splay_tree, zig_zag_left) {
    // splaying root
    STree tree;
    tree.InitPreorder({6, 2, 1, 4, 3, 5, 7});

    tree.Splay(tree.GetRoot()->GetLeft()->GetRight());

    ASSERT_TRUE(tree.IsValid());

    TestVector({4, 2, 1, 3, 6, 5, 7}, tree.OutputPreorder());
}

TEST(splay_tree, zig_zag_right) {
    // splaying root
    STree tree;
    tree.InitPreorder({2, 1, 6, 4, 3, 5, 7});

    tree.Splay(tree.GetRoot()->GetRight()->GetLeft());

    ASSERT_TRUE(tree.IsValid());
    TestVector({4, 2, 1, 3, 6, 5, 7}, tree.OutputPreorder());
}

TEST(splay_tree, search) {
    STree tree;
    tree.InitPreorder({2, 1, 3});
    ASSERT_TRUE(tree.IsValid());

    ASSERT_EQ(2, tree.Search(2)->GetData());
    ASSERT_TRUE(tree.IsValid());
    ASSERT_EQ(3, tree.Search(3)->GetData());
    ASSERT_TRUE(tree.IsValid());
    ASSERT_EQ(1, tree.Search(1)->GetData());
    ASSERT_TRUE(tree.IsValid());
    ASSERT_EQ(nullptr, tree.Search(4));
    ASSERT_TRUE(tree.IsValid());

    BSTree bigger_tree;
    tree.InitPreorder({10, 4, 2, 4, 10});
    ASSERT_EQ(10, tree.Search(10)->GetData());
    ASSERT_TRUE(tree.IsValid());
    ASSERT_EQ(4, tree.Search(4)->GetData());
    ASSERT_TRUE(tree.IsValid());
    ASSERT_EQ(nullptr, tree.Search(1));
    ASSERT_TRUE(tree.IsValid());
}
