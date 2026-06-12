#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

#include "Detail.hpp"

namespace ds {

template <typename T, typename Compare = std::less<T>, typename KeyOf = Identity<T>>
class AVL {
private:
    struct Node {
        T value;
        Node* left = nullptr;
        Node* right = nullptr;
        int height = 1;

        explicit Node(const T& v) : value(v) {}
        explicit Node(T&& v) : value(std::move(v)) {}
    };

    using Key = std::decay_t<decltype(std::declval<KeyOf>()(std::declval<T>()))>;

    Node* root_ = nullptr;
    std::size_t size_ = 0;
    Compare compare_{};
    KeyOf keyOf_{};

    static int height(Node* node) { return node ? node->height : 0; }

    static int balance_factor(Node* node) {
        return node ? height(node->left) - height(node->right) : 0;
    }

    void update_height(Node* node) {
        node->height = 1 + std::max(height(node->left), height(node->right));
    }

    bool less(const Key& lhs, const Key& rhs) const { return compare_(lhs, rhs); }

    Node* rotate_right(Node* node) {
        Node* left = node->left;
        Node* transferred = left->right;
        left->right = node;
        node->left = transferred;
        update_height(node);
        update_height(left);
        return left;
    }

    Node* rotate_left(Node* node) {
        Node* right = node->right;
        Node* transferred = right->left;
        right->left = node;
        node->right = transferred;
        update_height(node);
        update_height(right);
        return right;
    }

    Node* rebalance(Node* node) {
        update_height(node);
        int factor = balance_factor(node);
        if (factor > 1) {
            if (balance_factor(node->left) < 0) {
                node->left = rotate_left(node->left);
            }
            return rotate_right(node);
        }
        if (factor < -1) {
            if (balance_factor(node->right) > 0) {
                node->right = rotate_right(node->right);
            }
            return rotate_left(node);
        }
        return node;
    }

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
            node->value = std::move(value);
            return node;
        }
        return rebalance(node);
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
            if (node->left == nullptr || node->right == nullptr) {
                Node* child = node->left ? node->left : node->right;
                delete node;
                return child;
            }
            Node* successor = min_node(node->right);
            node->value = successor->value;
            node->right = erase_node(node->right, keyOf_(successor->value), erased);
        }
        return rebalance(node);
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
    AVL() = default;
    ~AVL() { clear(); }

    bool empty() const noexcept { return size_ == 0; }
    std::size_t size() const noexcept { return size_; }

    void clear() {
        clear_node(root_);
        root_ = nullptr;
        size_ = 0;
    }

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
        return const_cast<AVL*>(this)->find(key);
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