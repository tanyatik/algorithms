#include <vector>
#include <stack>

template<typename TElem>
struct TraitsSentinel;

template<typename TElem>
class BinarySearchTree {
    public:
        BinarySearchTree();
        void initPreorderRecursion(std::vector<TElem> vec);
        void initPreorderNoRecursion(std::vector<TElem> vec);
        void outputInorder() const;
        std::vector<TElem> outputPostorderNoRecursion() const;
        std::vector<TElem> outputPostorderRecursion() const;
        void debugPrint() const;

    private:
        struct Node {
            public:
                Node(TElem data);
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

                NodeTraverse() : 
                    node(nullptr), 
                    visited(false) {}
                NodeTraverse(Node *node) :
                    node(node),
                    visited(false) {}
        };

        Node *addNode(const TElem &value);
        Node *initPreorderInnerRecursion(typename std::vector<TElem>::iterator &iter, const TElem &max_value);
        Node *initPreorderInnerNoRecursion(typename std::vector<TElem>::iterator &iter, const TElem &max_value);

        void outputPostorderInnerRecursion(const Node *current, std::vector<TElem> *output) const;
        void outputPostorderInnerNoRecursion(std::vector<TElem> *output) const;
        //std::vector<Node> nodes_;
        Node nodes_[1000];
        Node *end_nodes_;
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
    std::cout << data_ << ' ';
}

template<typename TElem>
const TElem& BinarySearchTree<TElem>::Node::getData() const {
    return data_;   
}

template<typename TElem>
BinarySearchTree<TElem>::BinarySearchTree() {
    end_nodes_ = nodes_;
}

template<typename TElem>
void BinarySearchTree<TElem>::initPreorderRecursion(std::vector<TElem> vec) {
    auto max_sentinel = TraitsSentinel<TElem>::getMaxSentinel();
    vec.push_back(max_sentinel);
    auto iter = vec.begin();
    root_ = initPreorderInnerRecursion(iter, max_sentinel);
}

template<typename TElem>
void BinarySearchTree<TElem>::initPreorderNoRecursion(std::vector<TElem> vec) {
    auto max_sentinel = TraitsSentinel<TElem>::getMaxSentinel();
    vec.push_back(max_sentinel);
    auto iter = vec.begin();
    root_ = initPreorderInnerNoRecursion(iter, max_sentinel);
}

template<typename TElem>
typename BinarySearchTree<TElem>::Node *
BinarySearchTree<TElem>::initPreorderInnerRecursion(typename std::vector<TElem>::iterator &iter,
                                           const TElem &max_value) {
    Node *new_node = addNode(*iter++);
    if (*iter < max_value) {
        new_node->setLeft(initPreorderInnerRecursion(iter, new_node->getData()));
    }
    if (*iter < max_value) {
        new_node->setRight(initPreorderInnerRecursion(iter, max_value));
    }
    return new_node;
}

template<typename TElem>
typename BinarySearchTree<TElem>::Node *
BinarySearchTree<TElem>::initPreorderInnerNoRecursion(typename std::vector<TElem>::iterator &iter,
                                           const TElem &max_value) {
    std::stack<TElem> stack;
    stack.push(max_value);
    Node *current_node = addNode(*iter++);

    while(!stack.empty()) {
        if (!current_node->getLeft() && *iter < stack.top()) {
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
    return current_node;
}


template<typename TElem>
std::vector<TElem> BinarySearchTree<TElem>::outputPostorderRecursion() const {
    std::vector<TElem> output;
    outputPostorderInnerRecursion(root_, &output);
    return output;
}

template<typename TElem>
std::vector<TElem> BinarySearchTree<TElem>::outputPostorderNoRecursion() const {
    std::vector<TElem> output;
    outputPostorderInnerNoRecursion(&output);
    return output;
}

template<typename TElem>
void BinarySearchTree<TElem>::outputPostorderInnerRecursion(const Node *current, std::vector<TElem> *output) const {
    if (current->getLeft()) {
        outputPostorderInnerRecursion(current->getLeft(), output);
    }
    if (current->getRight()) {
        outputPostorderInnerRecursion(current->getRight(), output);
    }
    output->push_back(current->getData());
}


template<typename TElem>
void BinarySearchTree<TElem>::outputPostorderInnerNoRecursion(std::vector<TElem> *output) const {
    std::stack<Node *> stack;
    stack.push(root_, false);

    while(!stack.empty()) {
        if (stack.top()->getLeft()) {
            stack.push(std::make_pair(stack.top()->getLeft(), false));
            continue;
        }
        if (stack.top()->getRight()) {
            stack.push(std::make_pair(stack.top()->getRight(), false));
            continue;
        }
        output->push_back(stack.top()->getData());
        stack.pop();
    }
}

template<typename TElem>
typename BinarySearchTree<TElem>::Node *BinarySearchTree<TElem>::addNode(const TElem &value) {
    *end_nodes_ = Node(value);
    auto ret = end_nodes_;
    ++end_nodes_;
    return ret;
}

template<typename TElem>
void BinarySearchTree<TElem>::debugPrint() const {
    for(const Node *ptr = nodes_; ptr != end_nodes_; ++ptr) {
        ptr->debugPrint();
    }
    std::cout << std::endl;
}
