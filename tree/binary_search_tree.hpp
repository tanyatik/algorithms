#pragma once

#include <vector>
#include <array>
#include <stack>

namespace algorithms {

template<typename Elem>
struct TraitsSentinel;

template<typename Elem>
class BinarySearchTree {
protected:
    struct Node;
public:
    BinarySearchTree();
    void InitPreorderRecursion(std::vector<Elem> vec);
    void InitPreorder(std::vector<Elem> vec);
    std::vector<Elem> OutputPreorder() const;
    std::vector<Elem> OutputPostorder() const;
    std::vector<Elem> OutputPostorderRecursion() const;
    std::vector<Elem> OutputInorder() const;
    void DebugPrint() const;

    virtual Node* Insert(const Elem& elem);
    virtual void Remove(Node* to_remove) {
        throw std::runtime_error("Not implemented");
    }
    virtual Node* Search(const Elem& elem);

    Node* GetRoot() const { return root_; }

    bool IsValid() const;

protected:
    struct Node {
    public:
        Node(Elem data);
        Node();

        void SetLeft(Node *left);
        void SetRight(Node *right);
        const Node *GetLeft() const;
        const Node *GetRight() const;
        const Node *GetParent() const;
        Node *GetLeft();
        Node *GetRight();
        Node *GetParent();

        const Elem &GetData() const;

        void DebugPrint() const;

    private:
        Node *left_;
        Node *right_;
        Node *parent_;
        Elem data_;
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

    Node *AddNode(const Elem &value);
    Node *InitPreorderInnerRecursion(typename std::vector<Elem>::iterator &iter, const Elem &max_value);
    void InitPreorderInnerNoRecursion(std::vector<Elem> vec);

    void OutputPostorderInnerRecursion(const Node *current, std::vector<Elem> *output) const;
    void OutputPreorderInner(const Node *current, std::vector<Elem> *output) const;

    std::pair<Node*, bool> FindElem(const Elem& elem);

    bool IsValid(Node* node, Elem* min_val, Elem* max_val) const;

    std::array<Node, 1024/* * 128*/> nodes_;
    size_t end_nodes_index_;
    Node* root_;
};

template<typename Elem>
BinarySearchTree<Elem>::Node::Node() :
    left_(nullptr),
    right_(nullptr),
    parent_(nullptr),
    data_(Elem()) {}

template<typename Elem>
BinarySearchTree<Elem>::Node::Node(Elem data) :
    left_(nullptr),
    right_(nullptr),
    parent_(nullptr),
    data_(data) {}

template<typename Elem>
void BinarySearchTree<Elem>::Node::SetLeft(Node *left) {
    left_ = left;
    if (left) {
        left->parent_ = this;
    }
}

template<typename Elem>
void BinarySearchTree<Elem>::Node::SetRight(Node *right) {
    right_ = right;
    if (right) {
        right->parent_ = this;
    }
}

template<typename Elem>
typename BinarySearchTree<Elem>::Node const *BinarySearchTree<Elem>::Node::GetLeft() const {
    return left_;
}

template<typename Elem>
typename BinarySearchTree<Elem>::Node const *BinarySearchTree<Elem>::Node::GetRight() const {
    return right_;
}

template<typename Elem>
typename BinarySearchTree<Elem>::Node const *BinarySearchTree<Elem>::Node::GetParent() const {
    return parent_;
}

template<typename Elem>
typename BinarySearchTree<Elem>::Node *BinarySearchTree<Elem>::Node::GetLeft() {
    return left_;
}

template<typename Elem>
typename BinarySearchTree<Elem>::Node *BinarySearchTree<Elem>::Node::GetRight() {
    return right_;
}

template<typename Elem>
typename BinarySearchTree<Elem>::Node *BinarySearchTree<Elem>::Node::GetParent() {
    return parent_;
}

template<typename Elem>
void BinarySearchTree<Elem>::Node::DebugPrint() const {
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

template<typename Elem>
const Elem& BinarySearchTree<Elem>::Node::GetData() const {
    return data_;
}

template<typename Elem>
BinarySearchTree<Elem>::BinarySearchTree() {
    root_ = nullptr;
    end_nodes_index_ = 0;
}

template<typename Elem>
void BinarySearchTree<Elem>::InitPreorderRecursion(std::vector<Elem> vec) {
    auto max_sentinel = TraitsSentinel<Elem>::GetMaxSentinel();
    vec.push_back(max_sentinel);
    auto iter = vec.begin();
    root_ = InitPreorderInnerRecursion(iter, max_sentinel);
}

template<typename Elem>
void BinarySearchTree<Elem>::InitPreorderInnerNoRecursion(std::vector<Elem> vec) {
    auto max_sentinel = TraitsSentinel<Elem>::GetMaxSentinel();

    vec.push_back(max_sentinel);
    auto iter = vec.begin();

    std::stack<Elem> stack;
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

template<typename Elem>
void BinarySearchTree<Elem>::InitPreorder(std::vector<Elem> vec) {
    InitPreorderInnerNoRecursion(vec);
//    auto max_sentinel = TraitsSentinel<Elem>::getMaxSentinel();
//    vec.push_back(max_sentinel);
//
//    auto iter = vec.begin();
//    std::stack<Elem> stack;
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

template<typename Elem>
typename BinarySearchTree<Elem>::Node *
BinarySearchTree<Elem>::InitPreorderInnerRecursion(typename std::vector<Elem>::iterator &iter,
                                           const Elem &max_value) {
    Node *new_node = AddNode(*iter++);
    if (*iter < new_node->GetData()) {
        new_node->SetLeft(InitPreorderInnerRecursion(iter, new_node->GetData()));
    }
    if (*iter < max_value) {
        new_node->SetRight(InitPreorderInnerRecursion(iter, max_value));
    }
    return new_node;
}

template<typename Elem>
void BinarySearchTree<Elem>::OutputPostorderInnerRecursion(const Node *current, std::vector<Elem> *output) const {
    if (current->GetLeft()) {
        OutputPostorderInnerRecursion(current->GetLeft(), output);
    }
    if (current->GetRight()) {
        OutputPostorderInnerRecursion(current->GetRight(), output);
    }
    output->push_back(current->GetData());
}

template<typename Elem>
std::vector<Elem> BinarySearchTree<Elem>::OutputPostorderRecursion() const {
    std::vector<Elem> output;
    OutputPostorderInnerRecursion(root_, &output);
    return output;
}

template<typename Elem>
void BinarySearchTree<Elem>::OutputPreorderInner(const Node* current, std::vector<Elem>* output) const {
    output->push_back(current->GetData());
    if (current->GetLeft()) {
        OutputPreorderInner(current->GetLeft(), output);
    }
    if (current->GetRight()) {
        OutputPreorderInner(current->GetRight(), output);
    }
}

template<typename Elem>
std::vector<Elem> BinarySearchTree<Elem>::OutputPreorder() const {
    std::vector<Elem> output;
    OutputPreorderInner(root_, &output);
    return output;
}

template<typename Elem>
std::vector<Elem> BinarySearchTree<Elem>::OutputPostorder() const {
    std::vector<Elem> output;
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

template<typename Elem>
std::vector<Elem> BinarySearchTree<Elem>::OutputInorder() const {
    std::vector<Elem> output;
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


template<typename Elem>
std::pair<typename BinarySearchTree<Elem>::Node*, bool> BinarySearchTree<Elem>::FindElem(const Elem& elem) {
    Node* current = root_;
    Node* prev = root_;

    while (current) {
        if (current->GetData() == elem) {
            break;
        } else if (current->GetData() < elem) {
            prev = current;
            current = current->GetRight();
        } else {
            prev = current;
            current = current->GetLeft();
        }
    }

    if (current) {
        return std::make_pair(current, true);
    } else {
        return std::make_pair(prev, false);
    }
}


template<typename Elem>
typename BinarySearchTree<Elem>::Node* BinarySearchTree<Elem>::Search(const Elem& elem) {
    auto found = FindElem(elem);
    if (found.second) {
        return found.first;
    } else {
        return nullptr;
    }
}


template<typename Elem>
typename BinarySearchTree<Elem>::Node* BinarySearchTree<Elem>::Insert(const Elem& elem) {
    auto found = FindElem(elem);
    if (found.second) {
        return found.first;
    } else {
        Node* parent = found.first;
        if (!parent) {
            root_ = new Node(elem);
            return root_;
        } else if (elem < parent->GetData()) {
            assert(!parent->GetLeft());
            parent->SetLeft(new Node(elem));
            return parent->GetLeft();
        } else {
            assert(!parent->GetRight());
            parent->SetRight(new Node(elem));
            return parent->GetRight();
        }
    }
}


template<typename Elem>
typename BinarySearchTree<Elem>::Node* BinarySearchTree<Elem>::AddNode(const Elem &value) {
    nodes_[end_nodes_index_] = Node(value);
    ++end_nodes_index_;
    return &nodes_[end_nodes_index_ - 1];
}

template<typename Elem>
bool BinarySearchTree<Elem>::IsValid() const {
    int min_unused, max_unused;
    bool valid = IsValid(root_, &min_unused, &max_unused);
    if (!valid) {
        DebugPrint();
    }
    return valid;
}

template<typename Elem>
bool BinarySearchTree<Elem>::IsValid(Node* node, Elem* min_value, Elem* max_value) const {
    if (!node) return true;

    // *min_value = root->val;
    // *max_value = root->val;

    Elem minLeft, maxLeft;
    if (node->GetLeft()) {
        if (!IsValid(node->GetLeft(), &minLeft, &maxLeft) || maxLeft > node->GetData()) {
            return false;
        } else {
            *min_value = minLeft;
        }
    } else {
        *min_value = node->GetData();
    }

    int minRight, maxRight;
    if (node->GetRight()) {
        if (!IsValid(node->GetRight(), &minRight, &maxRight) || minRight < node->GetData()) {
            return false;
        } else {
            *max_value = maxRight;
        }
    } else {
        *max_value = node->GetData();
    }

    return true;
}

template<typename Elem>
void BinarySearchTree<Elem>::DebugPrint() const {
//    for(const Node *ptr = nodes_; ptr != end_nodes_; ++ptr) {
//        ptr->DebugPrint();
//    }
    root_->DebugPrint();
    std::cout << std::endl;
}

} //  namespace algorithms
