#include <vector>

template<typename TElem>
struct TraitsSentinel;

template<typename TElem>
class BinarySearchTree {
    public:
        BinarySearchTree();
        void initPreorder(std::vector<TElem> vec);

    private:
        struct Node {
            public:
                Node(TElem data);
                Node();
                
                void setLeft(Node *left);
                void setRight(Node *right);
                const TElem &getData() const;

                void debugPrint() const;
            private:
                Node *left_;
                Node *right_;
                Node *parent_;
                TElem data_;
        };

        Node *addNode(const TElem &value);
        Node *initPreorderInner(typename std::vector<TElem>::iterator &iter, const TElem &max_value);
        void debugPrint() const;

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
void BinarySearchTree<TElem>::initPreorder(std::vector<TElem> vec) {
    auto max_sentinel = TraitsSentinel<TElem>::getMaxSentinel();
    vec.push_back(max_sentinel);
    auto iter = vec.begin();
    root_ = initPreorderInner(iter, max_sentinel);
}

template<typename TElem>
typename BinarySearchTree<TElem>::Node *
BinarySearchTree<TElem>::initPreorderInner(typename std::vector<TElem>::iterator &iter,
                                           const TElem &max_value) {
    Node *new_node = addNode(*iter++);
    if (*iter < max_value) {
        new_node->setLeft(initPreorderInner(iter, new_node->getData()));
    }
    if (*iter < max_value) {
        new_node->setRight(initPreorderInner(iter, max_value));
    }
    return new_node;
}

template<typename TElem>
typename BinarySearchTree<TElem>::Node *
BinarySearchTree<TElem>::initPreorderNoRecursion(typename std::vector<TElem>::iterator &iter,
                                           const TElem &max_value) {
    std::stack<TElem> stack;
    stack.push_back(max_value);
    Node *current_node = new_node;
    while(!stack.empty()) {
        current_node = addNode(*iter++);
        if (*iter < stack.top()) {
            stack.push(current_node->getData());
            current_node = current_node->left();
            continue;
        }
        if (*iter < stack.top()) {
            stack.push(stack.top());
            current_node = current_node->right();
            continue;
        }
        stack.pop();
        current_node = current_node->parent();
    }
}

template<typename TElem>
typename BinarySearchTree<TElem>::Node *BinarySearchTree<TElem>::addNode(const TElem &value) {
    *end_nodes_ = Node(value);
    auto ret = end_nodes_;
    ++end_nodes_;
    return ret;
    //nodes_.push_back(Node(value));
    //return &nodes_.back();
}

template<typename TElem>
void BinarySearchTree<TElem>::debugPrint() const {
    for(Node *ptr = nodes_; ptr != end_nodes_; ++ptr) {
        ptr->debugPrint();
    }
}
/*
template<typename TElem>
void BinarySearchTree::initPreorder(std::vector<TElem> vec_preorder) {
    std::stack<TElem> stack;
    auto iter = vec_preorder.begin(); 

    stack.push(vec_preorder.front());
    *root_ = Node(*iter++);
    Node *current = root_;

    for (iter != vec_preorder.end()) {
        if(*iter < stack.top()) {
            addLeftChild(current);
            current = current->left();
            stack.push(*iter++);
            continue;
        } 
        if (*iter < stack.top()) {
            addRightChild(current);
            current = current->right();
            stack.push(*iter++);
            continue;
        }
        current->setData(stack.top());
        stack.pop();
        current = current->parent();
    }
}
*/
