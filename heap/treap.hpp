#include <functional>
#include <memory>
#include <random>
#include <stdexcept>

namespace algorithms {

template<typename TKey, typename TPriority = int>
class Treap {
private:
    struct TreapNode;

    typedef std::shared_ptr<TreapNode> TreapNodePointer;

    struct TreapNode {
        TreapNodePointer left_;
        TreapNodePointer right_;
        TKey key_;
        TPriority priority_;
    };

    void Split(TreapNodePointer node,
               const TKey &key,
               TreapNodePointer *result_left,
               TreapNodePointer *result_right);
    TreapNodePointer Merge(TreapNodePointer left, TreapNodePointer right);

    void Insert(TreapNodePointer *node, TreapNodePointer new_element);
    TreapNodePointer Find(TreapNodePointer node, const TKey &key) const;
    TreapNodePointer FindThisOrNext(TreapNodePointer node, const TKey &key) const;
    void Erase(TreapNodePointer *node, const TKey &key);

    TPriority CountRandomPriority() const;

    TreapNodePointer root_;
public:
    // return true if insertion is successful and there is no such element in the treap,
    // false otherwise
    bool Insert(const TKey &key);
    bool Erase(const TKey &key);

    bool Find(const TKey &key) const;
    bool FindThisOrNext(const TKey &key, TKey *result) const;

    // Throws std::exception if some property of binary search tree is unsatisfied
    void CheckStructure() const;
    void CheckStructure(TreapNodePointer node) const;
};

template<typename TKey, typename TPriority>
bool Treap<TKey, TPriority>::Find(const TKey &key) const {
    TreapNodePointer found = FindThisOrNext(root_, key);
    return (found != nullptr && found->key_ == key);
}

template<typename TKey, typename TPriority>
bool Treap<TKey, TPriority>::FindThisOrNext(const TKey &key, TKey *result) const {
    TreapNodePointer found = FindThisOrNext(root_, key);
    if (found == nullptr) {
        return false;
    } else {
        *result = found->key_;
        return true;
    }
}

template<typename TKey, typename TPriority>
typename Treap<TKey, TPriority>::TreapNodePointer Treap<TKey, TPriority>::FindThisOrNext
        (TreapNodePointer node, const TKey &key) const {
    if (node == nullptr) {
        return nullptr;
    } else if (node->key_ == key) {
        return node;
    } else if (node->key_ < key) {
        TreapNodePointer right_result = FindThisOrNext(node->right_, key);
        return right_result;
    } else {
        TreapNodePointer left_result = FindThisOrNext(node->left_, key);
        if (left_result == nullptr) {
            return node;
        } else {
            return left_result;
        }
    }
}

template<typename TKey, typename TPriority>
bool Treap<TKey, TPriority>::Insert(const TKey &key) {
    if (Find(key)) {
        return false;
    }

    TPriority new_priority = CountRandomPriority();
    TreapNode new_node = {nullptr, nullptr, key, new_priority};
    TreapNodePointer new_node_pointer(new TreapNode(new_node));

    Insert(&root_, new_node_pointer);
    return true;
}

template<typename TKey, typename TPriority>
void Treap<TKey, TPriority>::Split(TreapNodePointer node,
                                   const TKey &key,
                                   TreapNodePointer *result_left,
                                   TreapNodePointer *result_right) {
    if (node == nullptr) {
        *result_left = nullptr;
        *result_right = nullptr;
    } else if (node->key_ < key) {

        Split(node->right_, key, &node->right_, result_right);
        *result_left = node;
    } else {

        Split(node->left_, key, result_left, &node->left_);
        *result_right = node;
    }
}

template<typename TKey, typename TPriority>
typename Treap<TKey, TPriority>::TreapNodePointer Treap<TKey, TPriority>::Merge
        (TreapNodePointer left, TreapNodePointer right) {
    if (left != nullptr && right == nullptr) {
        return left;

    } else if (left == nullptr) {
        return right;

    } else if (left->key_ < right->key_) {
        right->left_ = Merge(left, right->left_);
        return right;

    } else {
        left->right_ = Merge(right, left->right_);
        return left;

    }
}

template<typename TKey, typename TPriority>
bool Treap<TKey, TPriority>::Erase(const TKey &key) {
    TreapNodePointer node_to_erase = FindThisOrNext(root_, key);

    if (node_to_erase == nullptr || node_to_erase->key_ != key) {
        return false;
    } else {
        Erase(&root_, key);
        return true;
    }
}


template<typename TKey, typename TPriority>
void Treap<TKey, TPriority>::Erase(TreapNodePointer *node, const TKey &key) {
    if ((*node)->key_ == key) {
        *node = Merge((*node)->left_, (*node)->right_);
    } else if ((*node)->key_ < key) {
        Erase(&(*node)->right_, key);
    } else {
        Erase(&(*node)->left_, key);
    }
}

template<typename TKey, typename TPriority>
TPriority Treap<TKey, TPriority>::CountRandomPriority() const {
    unsigned seed = 237;
    static std::minstd_rand0 generator (seed);

    TPriority max_priority = std::numeric_limits<TPriority>::max();
    return std::uniform_int_distribution<TPriority>(0, max_priority) (generator);
}

template<typename TKey, typename TPriority>
void Treap<TKey, TPriority>::Insert(TreapNodePointer *node, TreapNodePointer new_element) {
    if (*node == nullptr) {
        *node = new_element;

    } else if ((*node)->key_ == new_element->key_) {
        throw std::runtime_error("Duplicate key in treap\n");

    } else if ((*node)->priority_ < new_element->priority_) {
        Split(*node, new_element->key_, &new_element->left_, &new_element->right_);
        *node = new_element;

    } else {
        if ((*node)->key_ < new_element->key_) {
            Insert(&(*node)->right_, new_element);

        } else {
            Insert(&(*node)->left_, new_element);
        }
   }
}

template<typename TKey, typename TPriority>
void Treap<TKey, TPriority>::CheckStructure() const {
    CheckStructure(root_);
}

template<typename TKey, typename TPriority>
void Treap<TKey, TPriority>::CheckStructure(TreapNodePointer node) const {
    if (node == nullptr) {
        return;
    } else {
        CheckStructure(node->left_);
        CheckStructure(node->right_);

        if (node->left_ && node->left_->key_ >= node->key_) {
            std::string description = std::string("Key of left ") +
                                      std::to_string(node->left_->key_) +
                                      std::string(" is >= than key of root ") +
                                      std::to_string(node->key_) +
                                      std::string("\n");
            throw std::runtime_error(description);
        }
        if (node->right_ && node->right_->key_ < node->key_) {
            std::string description = std::string("Key of right ") +
                                      std::to_string(node->right_->key_) +
                                      std::string(" is < than key of root ") +
                                      std::to_string(node->key_) +
                                      std::string("\n");
            throw std::runtime_error(description);
        }
    }
}


} // namespace algorithms
