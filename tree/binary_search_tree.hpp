#pragma once

#include <vector>
#include <array>
#include <stack>

namespace algorithms {

template<typename Elem>
struct TraitsSentinel;

template<typename Elem>
class BinarySearchTree {
public:
    struct Node;

    BinarySearchTree();
    BinarySearchTree(Node* root)
        : root_(root) {
        if (root) {
            root->SetParent(nullptr);
        }
    }
    virtual ~BinarySearchTree();

    BinarySearchTree(const BinarySearchTree& other) = delete;
    BinarySearchTree(BinarySearchTree&& other) {
        root_ = other.root_;
        other.root_ = nullptr;
    }

    BinarySearchTree<Elem>& operator = (const BinarySearchTree<Elem>& other) = delete;
    BinarySearchTree<Elem>& operator = (BinarySearchTree<Elem>&& other) {
        if (root_) {
            FreeTree(root_);
        }
        root_ = other.root_;
        other.root_ = nullptr;
        return *this;
    }

    void InitPreorder(std::vector<Elem> vec);
    std::vector<Elem> OutputPreorder() const;
    std::vector<Elem> OutputPostorder() const;
    std::vector<Elem> OutputInorder() const;
    void DebugPrint() const;

    virtual Node* Insert(const Elem& elem);
    virtual void Delete(Node* to_remove);
    virtual void Delete(const Elem& to_remove);
    virtual Node* Search(const Elem& elem);

    Node* GetRoot() const { return root_; }

    bool IsValid() const;

    struct Node {
    public:
        Node(Elem data);
        Node();

        void SetLeft(Node *left);
        void SetRight(Node *right);
        void SetParent(Node *parent);
        const Node *GetLeft() const;
        const Node *GetRight() const;
        const Node *GetParent() const;
        Node *GetLeft();
        Node *GetRight();
        Node *GetParent();

        const Elem &GetData() const;
        void SwapData(Node *other);

        void DebugPrint() const;

    private:
        Node *left_;
        Node *right_;
        Node *parent_;
        Elem data_;
    };

protected:
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

    void OutputPreorder(const Node *current, std::vector<Elem> *output) const;

    Node *GetPredecessor(Node* current);
    Node *GetSuccessor(Node* current);

    void FreeNode(Node* node);
    void FreeTree(Node* root);

    std::pair<Node*, bool> FindElem(const Elem& elem);

    bool IsValid(Node* node, Elem* min_val, Elem* max_val) const;

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
    /*if (left_) {
        left_->SetParent(nullptr);
    }
    if (left && left->GetParent()) {
        assert(left->GetParent()->GetLeft() == left || left->GetParent()->GetRight() == left);
        if (left->GetParent()->GetLeft() == left) {
            left->GetParent()->SetLeft(nullptr);
        } else if (left->GetParent()->GetRight() == left) {
            left->GetParent()->SetRight(nullptr);
        }
    }*/
    left_ = left;
    if (left) {
        left->parent_ = this;
    }
}

template<typename Elem>
void BinarySearchTree<Elem>::Node::SetParent(Node *parent) {
    parent_ = parent;
}

template<typename Elem>
void BinarySearchTree<Elem>::Node::SetRight(Node *right) {
    /*
    if (right_) {
        right_->SetParent(nullptr);
    }

    if (right && right->GetParent()) {
        assert(right->GetParent()->GetLeft() == right || right->GetParent()->GetRight() == right);
        if (right->GetParent()->GetRight() == right) {
            right->GetParent()->SetRight(nullptr);
        } else if (right->GetParent()->GetRight() == right) {
            right->GetParent()->SetRight(nullptr);
        }
    }
    */
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
void BinarySearchTree<Elem>::Node::SwapData(Node *other) {
    using std::swap;
    swap(data_, other->data_);
}

template<typename Elem>
BinarySearchTree<Elem>::BinarySearchTree() {
    root_ = nullptr;
}

template<typename Elem>
BinarySearchTree<Elem>::~BinarySearchTree() {
    FreeTree(root_);
}

template<typename Elem>
void BinarySearchTree<Elem>::FreeTree(Node* root) {
    if (root) {
        FreeTree(root->GetLeft());
        FreeTree(root->GetRight());

        FreeNode(root);
    }
}


template<typename Elem>
void BinarySearchTree<Elem>::InitPreorder(std::vector<Elem> vec) {
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
void BinarySearchTree<Elem>::OutputPreorder(const Node* current, std::vector<Elem>* output) const {
    output->push_back(current->GetData());
    if (current->GetLeft()) {
        OutputPreorder(current->GetLeft(), output);
    }
    if (current->GetRight()) {
        OutputPreorder(current->GetRight(), output);
    }
}

template<typename Elem>
std::vector<Elem> BinarySearchTree<Elem>::OutputPreorder() const {
    std::vector<Elem> output;
    OutputPreorder(root_, &output);
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
    if (root_) {
        stack.push(NodeTraverse(root_));
    }

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
            root_ = AddNode(elem);
            return root_;
        } else if (elem < parent->GetData()) {
            assert(!parent->GetLeft());
            parent->SetLeft(AddNode(elem));
            return parent->GetLeft();
        } else {
            assert(!parent->GetRight());
            parent->SetRight(AddNode(elem));
            return parent->GetRight();
        }
    }
}

template<typename Elem>
void BinarySearchTree<Elem>::Delete(const Elem& elem) {
    typename BinarySearchTree<Elem>::Node* node = Search(elem);
    Delete(node);
}


template<typename Elem>
void BinarySearchTree<Elem>::Delete(BinarySearchTree<Elem>::Node* to_delete) {
    if (!to_delete) {
        return;
    }
    if (!to_delete->GetLeft() || !to_delete->GetRight()) {
        Node* child = to_delete->GetLeft();
        if (!child) {
            child = to_delete->GetRight();
        }

        if (!child) {
            FreeNode(to_delete);
            return;
        } else if (child && !child->GetLeft() && !child->GetRight()) {
            to_delete->SwapData(child);
            FreeNode(child);
            return;
        }
    }

    Node* swap_to = GetPredecessor(to_delete);
    if (!swap_to) {
        swap_to = GetSuccessor(to_delete);
    }
    if (swap_to) {
        swap_to->SwapData(to_delete);
        Delete(swap_to);
    }
}


template<typename Elem>
typename BinarySearchTree<Elem>::Node* BinarySearchTree<Elem>::AddNode(const Elem &value) {
    Node* node = new Node(value);
    // printf("new %llx\n", (unsigned long long) (void*) node);
    return node;
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

    assert(!node->GetLeft()  || node->GetLeft()->GetParent()  == node);
    assert(!node->GetRight() || node->GetRight()->GetParent() == node);

    Elem minLeft, maxLeft;
    if (node->GetLeft()) {
        if (!IsValid(node->GetLeft(), &minLeft, &maxLeft)) {
            return false;
        } else if (maxLeft > node->GetData()) {
            return false;
        } else {
            *min_value = minLeft;
        }
    } else {
        *min_value = node->GetData();
    }

    int minRight, maxRight;
    if (node->GetRight()) {
        if (!IsValid(node->GetRight(), &minRight, &maxRight)) {
            return false;
        } else if (minRight < node->GetData()) {
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
typename BinarySearchTree<Elem>::Node *BinarySearchTree<Elem>::GetPredecessor(Node* node) {
    Node* current = node->GetLeft();
    while (current && current->GetRight()) {
        current = current->GetRight();
    }
    return current;
}

template<typename Elem>
typename BinarySearchTree<Elem>::Node *BinarySearchTree<Elem>::GetSuccessor(Node* node) {
    Node* current = node->GetRight();
    while (current && current->GetLeft()) {
        current = current->GetLeft();
    }
    return current;
}

template<typename Elem>
void BinarySearchTree<Elem>::FreeNode(Node* node) {
    if (!node) {
        return;
    }
    if (node == root_) {
        delete root_;
        root_ = nullptr;
        return;
    }
    if (node->GetLeft()) {
        node->GetLeft()->SetParent(nullptr);
    }
    if (node->GetRight()) {
        node->GetRight()->SetParent(nullptr);
    }
    if (node->GetParent()) {
        if (node->GetParent()->GetLeft() == node) {
            node->GetParent()->SetLeft(nullptr);
        } else if (node->GetParent()->GetRight() == node) {
            node->GetParent()->SetRight(nullptr);
        }
    }
    /*
    printf("delete %llx (%d)\n", (unsigned long long) (void*) node, node->GetData());
    node->SetLeft(nullptr);
    node->SetRight(nullptr);
    node->SetParent(nullptr);
    */
    delete node;
}

template<typename Elem>
void BinarySearchTree<Elem>::DebugPrint() const {
    if (root_) {
        root_->DebugPrint();
    }
    std::cout << std::endl;
}

} //  namespace algorithms
