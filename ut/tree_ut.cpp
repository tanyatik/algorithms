#include <algorithm>
#include <set>
#include <random>
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

void TestTreeLikeSet(BSTree& tree, int test_size = 10000) {
    std::minstd_rand0 generator(std::time(nullptr));
    std::vector<int> data = InitRandomVector(&generator, -1000, 1000, test_size);

    std::set<int> set;

    for (int d : data) {
        tree.Insert(d);
        set.insert(d);

        ASSERT_TRUE(tree.IsValid());
        TestVector(tree.OutputInorder(), std::vector<int>(set.begin(), set.end()));
    }

    std::random_shuffle(data.begin(), data.end());

    for (int d : data) {
        tree.Delete(d);
        set.erase(d);

        ASSERT_TRUE(tree.IsValid());
        TestVector(tree.OutputInorder(), std::vector<int>(set.begin(), set.end()));
    }
}

TEST(binary_search_tree, preorder) {
    std::vector<int> keys_preorder{5, 2, 1, 3, 6, 7, 9};
    BSTree tree;

    tree.InitPreorder({5, 6});
}

TEST(binary_search_tree, postorder1) {
    {
        std::vector<int> keys_preorder{5, 2, 1, 3, 6, 7, 9};
        BSTree tree;
        tree.InitPreorder(keys_preorder);
        std::vector<int> postorder = tree.OutputPostorder();
        TestVector(postorder, {1, 3, 2, 9, 7, 6, 5});
        std::vector<int> postorder2 = tree.OutputPostorder();
        TestVector(postorder2, {1, 3, 2, 9, 7, 6, 5});
    }

    {
        BSTree tree = BSTree();
        tree.InitPreorder({4, 2, 1, 3, 6, 5, 7});
        std::vector<int> postorder = tree.OutputPostorder();
        TestVector(postorder, {1, 3, 2, 5, 7, 6, 4});
    }

    {
        BSTree tree = BSTree();
        tree.InitPreorder({5, 3, 2, 3, 5, 6});
        std::vector<int> postorder = tree.OutputPostorder();
        TestVector(postorder, {2, 3, 3, 6, 5, 5});
    }
}

TEST(binary_search_tree, inorder1) {
    {
        std::vector<int> keys_preorder{5, 2, 1, 3, 6, 7, 9};
        BSTree tree;
        tree.InitPreorder(keys_preorder);
        std::vector<int> inorder = tree.OutputInorder();
    }

    {
        BSTree tree = BSTree();
        tree.InitPreorder({4, 2, 1, 3, 6, 5, 7});
        std::vector<int> inorder = tree.OutputInorder();
        TestVector(inorder, {1, 2, 3, 4, 5, 6, 7});
    }

    {
        BSTree tree = BSTree();
        tree.InitPreorder({5, 3, 2, 3, 5, 6});
        std::vector<int> inorder = tree.OutputInorder();
        TestVector({2, 3, 3, 5, 5, 6}, inorder);
    }
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
    bigger_tree.InitPreorder({10, 4, 2, 4, 10});
    ASSERT_EQ(10, bigger_tree.Search(10)->GetData());
    ASSERT_EQ(4, bigger_tree.Search(4)->GetData());
    ASSERT_EQ(nullptr, bigger_tree.Search(1));
}


TEST(binary_search_tree, insert) {
    BSTree tree;

    tree.Insert(10);
    ASSERT_EQ(tree.GetRoot(), tree.Search(10));

    tree.Insert(7);
    ASSERT_EQ(7, tree.Search(7)->GetData());
    ASSERT_TRUE(tree.IsValid());

    tree.Insert(14);
    ASSERT_EQ(14, tree.Search(14)->GetData());
    ASSERT_TRUE(tree.IsValid());

    tree.Insert(3);
    ASSERT_EQ(3, tree.Search(3)->GetData());
    ASSERT_TRUE(tree.IsValid());
}

TEST(binary_search_tree, delet) {
    {
        BSTree tree;
        tree.Insert(10);
        tree.Insert(7);
        tree.Insert(14);
        tree.Delete(tree.GetRoot()->GetLeft());

        ASSERT_EQ(tree.GetRoot()->GetData(), 10);
        ASSERT_EQ(tree.GetRoot()->GetRight()->GetData(), 14);
        ASSERT_TRUE(!tree.GetRoot()->GetLeft());
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(tree.GetRoot()->GetRight());
        ASSERT_EQ(tree.GetRoot()->GetData(), 10);
        ASSERT_TRUE(!tree.GetRoot()->GetLeft());
        ASSERT_TRUE(!tree.GetRoot()->GetRight());
        ASSERT_TRUE(tree.IsValid());
    }
    {
        BSTree tree;
        tree.Insert(0);
        tree.Insert(6);
        tree.Insert(8);
        tree.Insert(7);

        tree.Delete(6);

        ASSERT_TRUE(tree.IsValid());
    }

    {
        BSTree tree;
        tree.InitPreorder({6, 5, 1, 4, 3, 2, 10, 7, 9, 8, 11});
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(tree.GetRoot()->GetRight()->GetRight());
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(tree.GetRoot()->GetRight());
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(tree.GetRoot()->GetLeft());
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(tree.GetRoot()->GetLeft());
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(tree.GetRoot());
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(tree.GetRoot()->GetRight());
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(tree.GetRoot()->GetLeft());
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(tree.GetRoot()->GetRight());
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(tree.GetRoot());
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(tree.GetRoot()->GetRight());
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(tree.GetRoot());
        ASSERT_TRUE(tree.IsValid());
    }
}

TEST(binary_search_tree, set) {
    BSTree tree;
    TestTreeLikeSet(tree);
}

TEST(splay_tree, zig_left) {
    STree tree;
    tree.InitPreorder({4, 2, 1, 3, 5});
    ASSERT_TRUE(tree.IsValid());

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
    TestVector({2, 1, 8, 4, 3, 6, 5, 7, 9}, bigger_tree.OutputPreorder());
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
    TestVector({6, 4, 2, 1, 3, 5, 8, 7, 9}, bigger_tree.OutputPreorder());
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

    STree bigger_tree;
    bigger_tree.InitPreorder({10, 4, 2, 4, 10});
    ASSERT_EQ(10, bigger_tree.Search(10)->GetData());
    ASSERT_TRUE(bigger_tree.IsValid());
    ASSERT_EQ(4, bigger_tree.Search(4)->GetData());
    ASSERT_TRUE(bigger_tree.IsValid());
    ASSERT_EQ(nullptr, bigger_tree.Search(1));
    ASSERT_TRUE(bigger_tree.IsValid());
}

TEST(splay_tree, delet) {
    {
        STree tree;
        tree.InitPreorder({6, 5, 1, 4, 3, 2, 10, 7, 9, 8, 11});
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(11);
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(10);
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(5);
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(1);
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(6);
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(7);
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(4);
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(8);
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(9);
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(3);
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(2);
        ASSERT_TRUE(tree.IsValid());
    }
    {
        STree tree;
        tree.Insert(10);
        tree.Insert(7);
        tree.Insert(14);
        tree.Delete(7);

        ASSERT_EQ(tree.GetRoot()->GetData(), 10);
        ASSERT_EQ(tree.GetRoot()->GetRight()->GetData(), 14);
        ASSERT_TRUE(!tree.GetRoot()->GetLeft());
        ASSERT_TRUE(tree.IsValid());

        tree.Delete(14);
        ASSERT_EQ(tree.GetRoot()->GetData(), 10);
        ASSERT_TRUE(!tree.GetRoot()->GetLeft());
        ASSERT_TRUE(!tree.GetRoot()->GetRight());
        ASSERT_TRUE(tree.IsValid());
    }
    {
        STree tree;
        tree.Insert(0);
        tree.Insert(6);
        tree.Insert(8);
        tree.Insert(7);

        tree.Delete(6);

        ASSERT_TRUE(tree.IsValid());
    }
}

TEST(splay_tree, set) {
    STree tree;
    TestTreeLikeSet(tree);
}

