#pragma once

#include "tree/binary_search_tree.hpp"

namespace algorithms {

template<typename Elem>
class SplayTree : public BinarySearchTree<Elem> {
public:
    typedef typename BinarySearchTree<Elem>::Node Node;
    // virtual Node* Insert(const Elem& elem);
    // virtual void Remove(Node* to_remove);
    virtual Node* Search(const Elem& elem) {
        Node* found = BinarySearchTree<Elem>::Search(elem);
        found = Splay(found);
        return found;
    }

    virtual Node* Insert(const Elem& elem) {
        Node* inserted = BinarySearchTree<Elem>::Search(elem);
        inserted = Splay(inserted);
        return inserted;
    }

public:
    // Move x to the root, performing rotations
    Node* Splay(Node* x) {
        if (!x || x == this->root_) {
            return x;
        } else if (x->GetParent() == this->root_) { // zig
            Node* prev_root = this->root_;

            if (this->root_->GetLeft() == x) { // left-hand
                Node* alpha = x->GetLeft();
                Node* beta = x->GetRight();
                Node* gamma = this->root_->GetRight();

                prev_root->SetLeft(beta);
                prev_root->SetRight(gamma);

                this->root_ = x;
                this->root_->SetLeft(alpha);
                this->root_->SetRight(prev_root);
            } else {
                Node* alpha = this->root_->GetLeft();
                Node* beta = x->GetLeft();
                Node* gamma = x->GetRight();

                prev_root->SetLeft(alpha);
                prev_root->SetRight(beta);

                this->root_ = x;
                this->root_->SetLeft(prev_root);
                this->root_->SetRight(gamma);
            }
        } else {
            Node* parent = x->GetParent();
            Node* parent_parent = parent->GetParent();
            Node* prev_root = parent_parent->GetParent();

            auto move_to_root = [&]() {
                if (parent_parent == this->root_) {
                    this->root_ = x;
                } else if (prev_root->GetLeft() == parent_parent) {
                    prev_root->SetLeft(x);
                    Splay(prev_root);
                } else if (prev_root->GetRight() == parent_parent) {
                    prev_root->SetRight(x);
                    Splay(prev_root);
                }
            };

            if (parent_parent->GetLeft() == parent && parent->GetLeft() == x) { // zig-zig left-hand
                // Node* alpha = x->GetLeft();
                Node* beta = x->GetRight();
                Node* gamma = parent->GetRight();
                Node* delta = parent_parent->GetRight();

                x->SetRight(parent);
                parent->SetLeft(beta);
                parent->SetRight(parent_parent);
                parent_parent->SetLeft(gamma);
                parent_parent->SetRight(delta);

                move_to_root();
            } else if (parent_parent->GetRight() == parent && parent->GetRight() == x) { // zig-zig right-hand
                // Node* alpha = parent_parent->GetLeft();
                Node* beta = parent->GetLeft();
                Node* gamma = x->GetLeft();
                Node* delta = x->GetRight();

                parent_parent->SetRight(beta);
                parent->SetLeft(parent_parent);
                parent->SetRight(gamma);
                x->SetLeft(parent);
                x->SetRight(delta);

                move_to_root();
            } else if (parent_parent->GetLeft() == parent && parent->GetRight() == x) { // zig-zag left-hand
                Node* alpha = parent->GetLeft();
                Node* beta = x->GetLeft();
                Node* gamma = x->GetRight();
                Node* delta = parent_parent->GetRight();

                parent->SetLeft(alpha);
                parent->SetRight(beta);
                parent_parent->SetLeft(gamma);
                parent_parent->SetRight(delta);
                x->SetLeft(parent);
                x->SetRight(parent_parent);

                move_to_root();
            } else { // zig-zag right-hand
                Node* beta = x->GetLeft();
                Node* gamma = x->GetRight();

                parent_parent->SetRight(beta);
                parent->SetLeft(gamma);
                x->SetLeft(parent_parent);
                x->SetRight(parent);

                move_to_root();
            }
        }
        return this->root_;
    }
    void Merge(Node* left, Node* right);
    void Split(Node* pivot, Node* left, Node* right);
};


} // namespace algorithms
