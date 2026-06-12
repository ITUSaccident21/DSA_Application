#pragma once

#include <cstddef>
#include <utility>

#include "LinkedList.hpp"

namespace ds {

template <typename T>
class Stack {
private:
    LinkedList<T> data_;

public:
    bool empty() const noexcept { return data_.empty(); }
    std::size_t size() const noexcept { return data_.size(); }

    void push(const T& value) { data_.push_front(value); }
    void push(T&& value) { data_.push_front(std::move(value)); }

    void pop() { data_.pop_front(); }

    T& top() { return data_.front(); }
    const T& top() const { return data_.front(); }

    void clear() { data_.clear(); }
};

} // namespace ds