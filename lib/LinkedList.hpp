#pragma once

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <utility>
#include <vector>

namespace ds {

template <typename T>
class LinkedList {
private:
    struct Node {
        T value;
        Node* next;

        explicit Node(const T& v, Node* n = nullptr) : value(v), next(n) {}
        explicit Node(T&& v, Node* n = nullptr) : value(std::move(v)), next(n) {}
    };

    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    std::size_t size_ = 0;

public:
    class iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator() = default;
        explicit iterator(Node* node) : node_(node) {}

        reference operator*() const { return node_->value; }
        pointer operator->() const { return &node_->value; }

        iterator& operator++() {
            node_ = node_ ? node_->next : nullptr;
            return *this;
        }

        iterator operator++(int) {
            iterator temp(*this);
            ++(*this);
            return temp;
        }

        friend bool operator==(const iterator& lhs, const iterator& rhs) { return lhs.node_ == rhs.node_; }
        friend bool operator!=(const iterator& lhs, const iterator& rhs) { return !(lhs == rhs); }

    private:
        Node* node_ = nullptr;
    };

    class const_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        const_iterator() = default;
        explicit const_iterator(const Node* node) : node_(node) {}
        const_iterator(const iterator& it) : node_(it.node_) {}

        reference operator*() const { return node_->value; }
        pointer operator->() const { return &node_->value; }

        const_iterator& operator++() {
            node_ = node_ ? node_->next : nullptr;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator temp(*this);
            ++(*this);
            return temp;
        }

        friend bool operator==(const const_iterator& lhs, const const_iterator& rhs) { return lhs.node_ == rhs.node_; }
        friend bool operator!=(const const_iterator& lhs, const const_iterator& rhs) { return !(lhs == rhs); }

    private:
        const Node* node_ = nullptr;
    };

    LinkedList() = default;

    LinkedList(std::initializer_list<T> values) {
        for (const auto& value : values) {
            push_back(value);
        }
    }

    LinkedList(const LinkedList& other) {
        for (const auto& value : other) {
            push_back(value);
        }
    }

    LinkedList(LinkedList&& other) noexcept { swap(other); }

    LinkedList& operator=(LinkedList other) noexcept {
        swap(other);
        return *this;
    }

    ~LinkedList() { clear(); }

    void swap(LinkedList& other) noexcept {
        using std::swap;
        swap(head_, other.head_);
        swap(tail_, other.tail_);
        swap(size_, other.size_);
    }

    bool empty() const noexcept { return size_ == 0; }
    std::size_t size() const noexcept { return size_; }

    iterator begin() noexcept { return iterator(head_); }
    iterator end() noexcept { return iterator(nullptr); }
    const_iterator begin() const noexcept { return const_iterator(head_); }
    const_iterator end() const noexcept { return const_iterator(nullptr); }
    const_iterator cbegin() const noexcept { return const_iterator(head_); }
    const_iterator cend() const noexcept { return const_iterator(nullptr); }

    T& front() {
        if (empty()) {
            throw std::out_of_range("LinkedList::front on empty list");
        }
        return head_->value;
    }

    const T& front() const {
        if (empty()) {
            throw std::out_of_range("LinkedList::front on empty list");
        }
        return head_->value;
    }

    T& back() {
        if (empty()) {
            throw std::out_of_range("LinkedList::back on empty list");
        }
        return tail_->value;
    }

    const T& back() const {
        if (empty()) {
            throw std::out_of_range("LinkedList::back on empty list");
        }
        return tail_->value;
    }

    void clear() noexcept {
        while (head_ != nullptr) {
            Node* next = head_->next;
            delete head_;
            head_ = next;
        }
        tail_ = nullptr;
        size_ = 0;
    }

    void push_front(const T& value) {
        head_ = new Node(value, head_);
        if (tail_ == nullptr) {
            tail_ = head_;
        }
        ++size_;
    }

    void push_front(T&& value) {
        head_ = new Node(std::move(value), head_);
        if (tail_ == nullptr) {
            tail_ = head_;
        }
        ++size_;
    }

    void push_back(const T& value) {
        Node* node = new Node(value);
        if (tail_ == nullptr) {
            head_ = tail_ = node;
        } else {
            tail_->next = node;
            tail_ = node;
        }
        ++size_;
    }

    void push_back(T&& value) {
        Node* node = new Node(std::move(value));
        if (tail_ == nullptr) {
            head_ = tail_ = node;
        } else {
            tail_->next = node;
            tail_ = node;
        }
        ++size_;
    }

    void pop_front() {
        if (empty()) {
            throw std::out_of_range("LinkedList::pop_front on empty list");
        }
        Node* next = head_->next;
        delete head_;
        head_ = next;
        if (head_ == nullptr) {
            tail_ = nullptr;
        }
        --size_;
    }

    // O(n): singly-linked; must traverse to find the node before tail.
    void pop_back() {
        if (empty()) {
            throw std::out_of_range("LinkedList::pop_back on empty list");
        }
        if (head_ == tail_) {
            delete head_;
            head_ = tail_ = nullptr;
            size_ = 0;
            return;
        }
        Node* current = head_;
        while (current->next != tail_) {
            current = current->next;
        }
        delete tail_;
        tail_ = current;
        tail_->next = nullptr;
        --size_;
    }

    T& at(std::size_t index) {
        return const_cast<T&>(std::as_const(*this).at(index));
    }

    const T& at(std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("LinkedList::at index out of range");
        }
        Node* current = head_;
        while (index-- > 0) {
            current = current->next;
        }
        return current->value;
    }

    T& operator[](std::size_t index) { return at(index); }
    const T& operator[](std::size_t index) const { return at(index); }

    iterator insert(std::size_t index, const T& value) {
        if (index > size_) {
            throw std::out_of_range("LinkedList::insert index out of range");
        }
        if (index == 0) {
            push_front(value);
            return begin();
        }
        if (index == size_) {
            push_back(value);
            Node* node = tail_;
            while (node != nullptr && node->next != tail_) {
                node = node->next;
            }
            return iterator(tail_);
        }
        Node* previous = head_;
        for (std::size_t i = 0; i < index - 1; ++i) {
            previous = previous->next;
        }
        previous->next = new Node(value, previous->next);
        ++size_;
        return iterator(previous->next);
    }

    iterator erase(std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("LinkedList::erase index out of range");
        }
        if (index == 0) {
            pop_front();
            return begin();
        }
        Node* previous = head_;
        for (std::size_t i = 0; i < index - 1; ++i) {
            previous = previous->next;
        }
        Node* target = previous->next;
        previous->next = target->next;
        if (target == tail_) {
            tail_ = previous;
        }
        delete target;
        --size_;
        return iterator(previous->next);
    }

    bool contains(const T& value) const {
        return find(value) != cend();
    }

    iterator find(const T& value) {
        for (auto it = begin(); it != end(); ++it) {
            if (*it == value) {
                return it;
            }
        }
        return end();
    }

    const_iterator find(const T& value) const {
        for (auto it = begin(); it != end(); ++it) {
            if (*it == value) {
                return const_iterator(it);
            }
        }
        return cend();
    }

    std::vector<T> to_vector() const {
        std::vector<T> values;
        values.reserve(size_);
        for (const auto& value : *this) {
            values.push_back(value);
        }
        return values;
    }
};

template <typename T>
void swap(LinkedList<T>& lhs, LinkedList<T>& rhs) noexcept {
    lhs.swap(rhs);
}

} // namespace ds
