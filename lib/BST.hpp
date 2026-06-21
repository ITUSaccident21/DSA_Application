#pragma once

#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

#include "Detail.hpp"

namespace ds {

template <typename T, typename Compare = std::less<T>, typename KeyOf = Identity<T>>
class BST {
private:
    struct Node {
        T value;
        Node* left = nullptr;
        Node* right = nullptr;

        explicit Node(const T& v) : value(v) {}
        explicit Node(T&& v) : value(std::move(v)) {}
    };

    using Key = std::decay_t<decltype(std::declval<KeyOf>()(std::declval<T>()))>;

    Node* root_ = nullptr;
    std::size_t size_ = 0;
    Compare compare_{};
    KeyOf keyOf_{};

    bool less(const Key& lhs, const Key& rhs) const { return compare_(lhs, rhs); }

    Node* insert_node(Node* node, T value, bool& inserted) {
        if (node == nullptr) {
            inserted = true;
            return new Node(std::move(value));
        }

        const auto& key = keyOf_(value);
        const auto& nodeKey = keyOf_(node->value);
        if (less(key, nodeKey)) {
            node->left = insert_node(node->left, std::move(value), inserted);
        } else if (less(nodeKey, key)) {
            node->right = insert_node(node->right, std::move(value), inserted);
        } else {
            node->value = std::move(value);  // duplicate key: overwrite existing value
        }
        return node;
    }

    Node* min_node(Node* node) const {
        while (node && node->left) {
            node = node->left;
        }
        return node;
    }

    Node* erase_node(Node* node, const Key& key, bool& erased) {
        if (node == nullptr) {
            return nullptr;
        }

        const auto& nodeKey = keyOf_(node->value);
        if (less(key, nodeKey)) {
            node->left = erase_node(node->left, key, erased);
        } else if (less(nodeKey, key)) {
            node->right = erase_node(node->right, key, erased);
        } else {
            erased = true;
            if (node->left == nullptr) {
                Node* right = node->right;
                delete node;
                return right;
            }
            if (node->right == nullptr) {
                Node* left = node->left;
                delete node;
                return left;
            }
            Node* successor = min_node(node->right);
            node->value = successor->value;
            node->right = erase_node(node->right, keyOf_(successor->value), erased);
        }
        return node;
    }

    void clear_node(Node* node) {
        if (!node) {
            return;
        }
        clear_node(node->left);
        clear_node(node->right);
        delete node;
    }

    template <typename Func>
    void inorder_node(Node* node, Func&& func) const {
        if (!node) {
            return;
        }
        inorder_node(node->left, func);
        func(node->value);
        inorder_node(node->right, func);
    }

public:
    BST() = default;
    ~BST() { clear(); }

    bool empty() const noexcept { return size_ == 0; }
    std::size_t size() const noexcept { return size_; }

    void clear() {
        clear_node(root_);
        root_ = nullptr;
        size_ = 0;
    }

    // Returns true if key is new, false if key existed (existing value is overwritten).
    // Warning: monotone key sequences produce a degenerate right- (or left-) skewed
    // tree with O(n) height, making all operations O(n). Use AVL if keys may arrive
    // in sorted order.
    bool insert(const T& value) {
        bool inserted = false;
        root_ = insert_node(root_, value, inserted);
        size_ += inserted ? 1 : 0;
        return inserted;
    }

    bool insert(T&& value) {
        bool inserted = false;
        root_ = insert_node(root_, std::move(value), inserted);
        size_ += inserted ? 1 : 0;
        return inserted;
    }

    bool remove(const Key& key) {
        bool erased = false;
        root_ = erase_node(root_, key, erased);
        size_ -= erased ? 1 : 0;
        return erased;
    }

    T* find(const Key& key) {
        Node* node = root_;
        while (node) {
            const auto& nodeKey = keyOf_(node->value);
            if (less(key, nodeKey)) {
                node = node->left;
            } else if (less(nodeKey, key)) {
                node = node->right;
            } else {
                return &node->value;
            }
        }
        return nullptr;
    }

    const T* find(const Key& key) const {
        return const_cast<BST*>(this)->find(key);
    }

    bool contains(const Key& key) const { return find(key) != nullptr; }

    T* min() {
        Node* node = min_node(root_);
        return node ? &node->value : nullptr;
    }

    T* max() {
        Node* node = root_;
        while (node && node->right) {
            node = node->right;
        }
        return node ? &node->value : nullptr;
    }

    template <typename Func>
    void inorder(Func&& func) const {
        inorder_node(root_, std::forward<Func>(func));
    }
};

} // namespace ds