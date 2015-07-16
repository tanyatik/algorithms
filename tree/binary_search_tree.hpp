#include <vector>
#include <array>
#include <stack>

namespace algorithms {

template<typename TElem>
struct TraitsSentinel;

template<typename TElem>
class BinarySearchTree {
public:
    BinarySearchTree();
    void InitPreorderRecursion(std::vector<TElem> vec);
    void InitPreorder(std::vector<TElem> vec);
    std::vector<TElem> OutputPostorder() const;
    std::vector<TElem> OutputPostorderRecursion() const;
    std::vector<TElem> OutputInorder() const;
    void DebugPrint() const;

private:
    struct Node {
    public:
        Node(TElem data);
        Node();

        void SetLeft(Node *left);
        void SetRight(Node *right);
        const Node *GetLeft() const;
        const Node *GetRight() const;
        const Node *GetParent() const;
        Node *GetLeft();
        Node *GetRight();
        Node *GetParent();

        const TElem &GetData() const;

        void DebugPrint() const;

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

        NodeTraverse(Node *node) :
            node(node),
            visited(false),
            visited_left(false),
            visited_right(false) {}
    };

    Node *AddNode(const TElem &value);
    Node *InitPreorderInnerRecursion(typename std::vector<TElem>::iterator &iter, const TElem &max_value);
    void InitPreorderInnerNoRecursion(std::vector<TElem> vec);

    void OutputPostorderInnerRecursion(const Node *current, std::vector<TElem> *output) const;
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
void BinarySearchTree<TElem>::Node::SetLeft(Node *left) {
    left_ = left;
    left->parent_ = this;
}

template<typename TElem>
void BinarySearchTree<TElem>::Node::SetRight(Node *right) {
    right_ = right;
    right->parent_ = this;
}

template<typename TElem>
typename BinarySearchTree<TElem>::Node const *BinarySearchTree<TElem>::Node::GetLeft() const {
    return left_;
}

template<typename TElem>
typename BinarySearchTree<TElem>::Node const *BinarySearchTree<TElem>::Node::GetRight() const {
    return right_;
}

template<typename TElem>
typename BinarySearchTree<TElem>::Node const *BinarySearchTree<TElem>::Node::GetParent() const {
    assert(parent_);
    return parent_;
}

template<typename TElem>
typename BinarySearchTree<TElem>::Node *BinarySearchTree<TElem>::Node::GetLeft() {
    return left_;
}

template<typename TElem>
typename BinarySearchTree<TElem>::Node *BinarySearchTree<TElem>::Node::GetRight() {
    return right_;
}

template<typename TElem>
typename BinarySearchTree<TElem>::Node *BinarySearchTree<TElem>::Node::GetParent() {
    assert(parent_);
    return parent_;
}

template<typename TElem>
void BinarySearchTree<TElem>::Node::DebugPrint() const {
    std::cout << data_;
    if (left_) {
        std::cout << ", L{ ";
        left_->DebugPrint();
        std::cout << "} ";
    }
    if (right_) {
        std::cout << ", R{ ";
        right_->DebugPrint();
        std::cout << "} ";
    }
}

template<typename TElem>
const TElem& BinarySearchTree<TElem>::Node::GetData() const {
    return data_;
}

template<typename TElem>
BinarySearchTree<TElem>::BinarySearchTree() {
    end_nodes_index_ = 0;
}

template<typename TElem>
void BinarySearchTree<TElem>::InitPreorderRecursion(std::vector<TElem> vec) {
    auto max_sentinel = TraitsSentinel<TElem>::GetMaxSentinel();
    vec.push_back(max_sentinel);
    auto iter = vec.begin();
    root_ = InitPreorderInnerRecursion(iter, max_sentinel);
}

template<typename TElem>
void BinarySearchTree<TElem>::InitPreorderInnerNoRecursion(std::vector<TElem> vec) {
    auto max_sentinel = TraitsSentinel<TElem>::GetMaxSentinel();

    vec.push_back(max_sentinel);
    auto iter = vec.begin();

    std::stack<TElem> stack;
    stack.push(max_sentinel);

    Node *current_node = AddNode(*iter++);
    root_ = current_node;

    while(!stack.empty()) {
        if (!current_node->GetLeft() && *iter < current_node->GetData()) {
            stack.push(current_node->GetData());
            current_node->SetLeft(AddNode(*iter++));
            current_node = current_node->GetLeft();
            continue;
        }
        if (*iter < stack.top()) {
            stack.push(stack.top());
            current_node->SetRight(AddNode(*iter++));
            current_node = current_node->GetRight();
            continue;
        }
        stack.pop();
        if (!stack.empty()) {
            current_node = current_node->GetParent();
        }
    }
}

template<typename TElem>
void BinarySearchTree<TElem>::InitPreorder(std::vector<TElem> vec) {
    InitPreorderInnerNoRecursion(vec);
//    auto max_sentinel = TraitsSentinel<TElem>::getMaxSentinel();
//    vec.push_back(max_sentinel);
//
//    auto iter = vec.begin();
//    std::stack<TElem> stack;
//    stack.push(max_sentinel);
//    //stack.push(*iter);
//
//    //root_ = AddNode(*iter++);
//    Node *current_node = root_;
//
//    while(!stack.empty()) {
//        current_node = AddNode(*iter++);
//
//        if (!current_node->GetLeft() && *iter < current_node->GetData()) {
//            //current_node->SetLeft(AddNode(*iter++));
//            stack.push(current_node->GetData());
//            current_node = current_node->GetLeft();
//            continue;
//        }
//
//        if (*iter < stack.top()) {
//            stack.push(stack.top());
//            //current_node->SetRight(AddNode(*iter++));
//            current_node = current_node->GetRight();
//            continue;
//        }
//        stack.pop();
//        if (!stack.empty()) {
//            current_node = current_node->GetParent();
//        }
//    }
}

template<typename TElem>
typename BinarySearchTree<TElem>::Node *
BinarySearchTree<TElem>::InitPreorderInnerRecursion(typename std::vector<TElem>::iterator &iter,
                                           const TElem &max_value) {
    Node *new_node = AddNode(*iter++);
    if (*iter < new_node->GetData()) {
        new_node->SetLeft(InitPreorderInnerRecursion(iter, new_node->GetData()));
    }
    if (*iter < max_value) {
        new_node->SetRight(InitPreorderInnerRecursion(iter, max_value));
    }
    return new_node;
}

template<typename TElem>
std::vector<TElem> BinarySearchTree<TElem>::OutputPostorderRecursion() const {
    std::vector<TElem> output;
    OutputPostorderInnerRecursion(root_, &output);
    return output;
}

template<typename TElem>
std::vector<TElem> BinarySearchTree<TElem>::OutputPostorder() const {
    std::vector<TElem> output;
    std::stack<NodeTraverse> stack;
    stack.push(NodeTraverse(root_));

    while(!stack.empty()) {
        if (stack.top().node->GetLeft() && !stack.top().visited_left) {
            stack.top().visited_left = true;
            stack.push(NodeTraverse(stack.top().node->GetLeft()));
            continue;
        }
        if (stack.top().node->GetRight() && !stack.top().visited_right) {
            stack.top().visited_right = true;
            stack.push(NodeTraverse(stack.top().node->GetRight()));
            continue;
        }
        output.push_back(stack.top().node->GetData());
        stack.pop();
    }
    return output;
}

template<typename TElem>
void BinarySearchTree<TElem>::OutputPostorderInnerRecursion(const Node *current, std::vector<TElem> *output) const {
    if (current->GetLeft()) {
        OutputPostorderInnerRecursion(current->GetLeft(), output);
    }
    if (current->GetRight()) {
        OutputPostorderInnerRecursion(current->GetRight(), output);
    }
    output->push_back(current->GetData());
}

template<typename TElem>
std::vector<TElem> BinarySearchTree<TElem>::OutputInorder() const {
    std::vector<TElem> output;
    std::stack<NodeTraverse> stack;
    stack.push(NodeTraverse(root_));

    while(!stack.empty()) {
        if (stack.top().node->GetLeft() && !stack.top().visited_left) {
            stack.top().visited_left = true;
            stack.push(NodeTraverse(stack.top().node->GetLeft()));
            continue;
        }
        if (!stack.top().visited) {
            output.push_back(stack.top().node->GetData());
            stack.top().visited = true;
        }
        if (stack.top().node->GetRight() && !stack.top().visited_right) {
            stack.top().visited_right = true;
            stack.push(NodeTraverse(stack.top().node->GetRight()));
            continue;
        }
        stack.pop();
    }
    return output;
}


template<typename TElem>
typename BinarySearchTree<TElem>::Node *BinarySearchTree<TElem>::AddNode(const TElem &value) {
    nodes_[end_nodes_index_] = Node(value);
    ++end_nodes_index_;
    return &nodes_[end_nodes_index_ - 1];
}

template<typename TElem>
void BinarySearchTree<TElem>::DebugPrint() const {
//    for(const Node *ptr = nodes_; ptr != end_nodes_; ++ptr) {
//        ptr->DebugPrint();
//    }
    root_->DebugPrint();
    std::cout << std::endl;
}

} //  namespace algorithms
