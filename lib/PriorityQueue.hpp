#pragma once

#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

namespace ds {

// Heap-based priority queue.
//
// Comparator semantics (opposite of std::priority_queue):
//   compare(a, b) = true  →  a sinks, b rises  (b has higher priority).
//
// With the default Compare = std::less<T>, larger values have higher priority
// (max-heap). To make smaller values have higher priority, pass std::greater<T>.
//
// When writing a custom comparator for type K:
//   "a should be processed BEFORE b"  →  compare(b, a) = true  (b sinks, a rises)
template <typename T, typename Compare = std::less<T>>
class PriorityQueue {
private:
    std::vector<T> heap_;
    Compare compare_{};

    void sift_up(std::size_t index) {
        while (index > 0) {
            std::size_t parent = (index - 1) / 2;
            if (!compare_(heap_[parent], heap_[index])) {
                break;
            }
            std::swap(heap_[parent], heap_[index]);
            index = parent;
        }
    }

    void sift_down(std::size_t index) {
        while (true) {
            std::size_t left = index * 2 + 1;
            std::size_t right = left + 1;
            std::size_t best = index;

            if (left < heap_.size() && compare_(heap_[best], heap_[left])) {
                best = left;
            }
            if (right < heap_.size() && compare_(heap_[best], heap_[right])) {
                best = right;
            }
            if (best == index) {
                break;
            }
            std::swap(heap_[index], heap_[best]);
            index = best;
        }
    }

public:
    PriorityQueue() = default;

    template <typename InputIt>
    PriorityQueue(InputIt first, InputIt last) {
        for (; first != last; ++first) {
            push(*first);
        }
    }

    bool empty() const noexcept { return heap_.empty(); }
    std::size_t size() const noexcept { return heap_.size(); }

    const T& top() const { return heap_.front(); }

    void push(const T& value) {
        heap_.push_back(value);
        sift_up(heap_.size() - 1);
    }

    void push(T&& value) {
        heap_.push_back(std::move(value));
        sift_up(heap_.size() - 1);
    }

    void pop() {
        if (heap_.empty()) {
            return;
        }
        std::swap(heap_.front(), heap_.back());
        heap_.pop_back();
        if (!heap_.empty()) {
            sift_down(0);
        }
    }

    void clear() { heap_.clear(); }
};

} // namespace ds