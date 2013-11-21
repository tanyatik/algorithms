#include <assert.h>
#include <iostream>
#include <algorithm>
#include <array>
#include <stack>
#include <vector>

template<typename TElem>
struct TraitsSentinel;

template<typename TElem>
class BinarySearchTree {
    public:
        BinarySearchTree();
        void initPreorderRecursion(std::vector<TElem> vec);
        void initPreorder(std::vector<TElem> vec);
        std::vector<TElem> outputPostorder() const;
        std::vector<TElem> outputPostorderRecursion() const;
        std::vector<TElem> outputInorder() const;
        void debugPrint() const;

    private:
        struct Node {
            public:
                explicit Node(TElem data);
                Node();
                
                void setLeft(Node *left);
                void setRight(Node *right);
                const Node *getLeft() const;
                const Node *getRight() const;
                const Node *getParent() const;
                Node *getLeft();
                Node *getRight();
                Node *getParent();

                const TElem &getData() const;

                void debugPrint() const;
            private:
                Node *left_;
                Node *right_;
                Node *parent_;
                TElem data_;
        };
        struct NodeTraverse {
            public:
                Node *node;
                bool visited;
                bool visited_left;
                bool visited_right;

                NodeTraverse() : 
                    node(nullptr), 
                    visited(false),
                    visited_left(false),
                    visited_right(false) {}
                explicit NodeTraverse(Node *node) :
                    node(node),
                    visited(false),
                    visited_left(false),
                    visited_right(false) {}
        };

        Node *addNode(const TElem &value);
        void initPreorderInnerNoRecursion(std::vector<TElem> vec);

        std::array<Node, 1024 * 128> nodes_;
        size_t end_nodes_index_;
        Node *root_;
};

template<typename TElem>
BinarySearchTree<TElem>::Node::Node() :
    left_(nullptr),
    right_(nullptr),
    parent_(nullptr), 
    data_(TElem()) {}

template<typename TElem>
BinarySearchTree<TElem>::Node::Node(TElem data) :
    left_(nullptr),
    right_(nullptr),
    parent_(nullptr), 
    data_(data) {}

template<typename TElem>
void BinarySearchTree<TElem>::Node::setLeft(Node *left) {
    left_ = left;
    left->parent_ = this;
}

template<typename TElem>
void BinarySearchTree<TElem>::Node::setRight(Node *right) {
    right_ = right;
    right->parent_ = this;
}

template<typename TElem>
typename BinarySearchTree<TElem>::Node const *BinarySearchTree<TElem>::Node::getLeft() const {
    return left_;
}

template<typename TElem>
typename BinarySearchTree<TElem>::Node const *BinarySearchTree<TElem>::Node::getRight() const {
    return right_;
}

template<typename TElem>
typename BinarySearchTree<TElem>::Node const *BinarySearchTree<TElem>::Node::getParent() const {
    assert(parent_);
    return parent_;
}

template<typename TElem>
typename BinarySearchTree<TElem>::Node *BinarySearchTree<TElem>::Node::getLeft() {
    return left_;
}

template<typename TElem>
typename BinarySearchTree<TElem>::Node *BinarySearchTree<TElem>::Node::getRight() {
    return right_;
}

template<typename TElem>
typename BinarySearchTree<TElem>::Node *BinarySearchTree<TElem>::Node::getParent() {
    assert(parent_);
    return parent_;
}

template<typename TElem>
void BinarySearchTree<TElem>::Node::debugPrint() const {
    std::cout << data_;
    if (left_) {
        std::cout << ", L{ ";
        left_->debugPrint();
        std::cout << "} ";
    }
    if (right_) {
        std::cout << ", R{ ";
        right_->debugPrint();
        std::cout << "} ";
    }
}

template<typename TElem>
const TElem& BinarySearchTree<TElem>::Node::getData() const {
    return data_;   
}

template<typename TElem>
BinarySearchTree<TElem>::BinarySearchTree() {
    end_nodes_index_ = 0;
}

template<typename TElem>
void BinarySearchTree<TElem>::initPreorder(std::vector<TElem> vec) {
    auto max_sentinel = TraitsSentinel<TElem>::getMaxSentinel();

    vec.push_back(max_sentinel);
    auto iter = vec.begin();

    std::stack<TElem> stack;
    stack.push(max_sentinel);

    Node *current_node = addNode(*iter++);
    root_ = current_node;

    while (!stack.empty()) {
        if (!current_node->getLeft() && *iter < current_node->getData()) {
            stack.push(current_node->getData());
            current_node->setLeft(addNode(*iter++));
            current_node = current_node->getLeft();
            continue;
        }
        if (*iter < stack.top()) {
            stack.push(stack.top());
            current_node->setRight(addNode(*iter++));
            current_node = current_node->getRight();
            continue;
        }
        stack.pop();
        if (!stack.empty()) {
            current_node = current_node->getParent();
        }
    }
}

template<typename TElem>
std::vector<TElem> BinarySearchTree<TElem>::outputPostorder() const {
    std::vector<TElem> output;
    std::stack<NodeTraverse> stack;
    stack.push(NodeTraverse(root_));

    while (!stack.empty()) {
        if (stack.top().node->getLeft() && !stack.top().visited_left) {
            stack.top().visited_left = true;
            stack.push(NodeTraverse(stack.top().node->getLeft()));
            continue;
        }
        if (stack.top().node->getRight() && !stack.top().visited_right) {
            stack.top().visited_right = true;
            stack.push(NodeTraverse(stack.top().node->getRight()));
            continue;
        }
        output.push_back(stack.top().node->getData());
        stack.pop();
    }
    return output;
}

template<typename TElem>
std::vector<TElem> BinarySearchTree<TElem>::outputInorder() const {
    std::vector<TElem> output;
    std::stack<NodeTraverse> stack;
    stack.push(NodeTraverse(root_));

    while (!stack.empty()) {
        if (stack.top().node->getLeft() && !stack.top().visited_left) {
            stack.top().visited_left = true;
            stack.push(NodeTraverse(stack.top().node->getLeft()));
            continue;
        }
        if (!stack.top().visited) {
            output.push_back(stack.top().node->getData());
            stack.top().visited = true;
        }
        if (stack.top().node->getRight() && !stack.top().visited_right) {
            stack.top().visited_right = true;
            stack.push(NodeTraverse(stack.top().node->getRight()));
            continue;
        }
        stack.pop();
    }
    return output;
}


template<typename TElem>
typename BinarySearchTree<TElem>::Node *BinarySearchTree<TElem>::addNode(const TElem &value) {
    nodes_[end_nodes_index_] = Node(value);
    ++end_nodes_index_;
    return &nodes_[end_nodes_index_ - 1];
}

template<typename TElem>
void BinarySearchTree<TElem>::debugPrint() const {
    root_->debugPrint();
    std::cout << std::endl;
}

template<>
struct TraitsSentinel<int> {
    static int getMinSentinel() { return -1000000001; }
    static int getMaxSentinel() { return  1000000001; }
};

int main() {
    BinarySearchTree<int> tree;
    int keys_number = 0;
    using std::cin;
    using std::cout;

    cin >> keys_number;
    std::vector<int> in(keys_number);
    for (auto it = in.begin(); it != in.end(); ++it) {
        std::cin >> *it;
    }

    tree.initPreorder(in);
    std::vector<int> out = tree.outputPostorder(); 
    for (auto it = out.begin(); it != out.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;

    out = tree.outputInorder(); 
    for (auto it = out.begin(); it != out.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;
    return 0;
}