#pragma once

#include <cstddef>
#include <utility>

#include "LinkedList.hpp"

namespace ds {

template <typename T>
class Queue {
private:
    LinkedList<T> data_;

public:
    bool empty() const noexcept { return data_.empty(); }
    std::size_t size() const noexcept { return data_.size(); }

    void push(const T& value) { data_.push_back(value); }
    void push(T&& value) { data_.push_back(std::move(value)); }

    void pop() { data_.pop_front(); }

    T& front() { return data_.front(); }
    const T& front() const { return data_.front(); }
    T& back() { return data_.back(); }
    const T& back() const { return data_.back(); }

    void clear() { data_.clear(); }
};

} // namespace ds