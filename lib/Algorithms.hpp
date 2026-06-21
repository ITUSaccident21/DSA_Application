#pragma once

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

namespace ds {

template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void bubbleSort(RandomIt first, RandomIt last, Compare compare = Compare{}) {
    if (first == last) {
        return;
    }
    for (auto end = last; end != first; --end) {
        for (auto it = first; std::next(it) != end; ++it) {
            auto next = std::next(it);
            if (compare(*next, *it)) {
                std::iter_swap(it, next);
            }
        }
    }
}

template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void selectionSort(RandomIt first, RandomIt last, Compare compare = Compare{}) {
    for (auto it = first; it != last; ++it) {
        auto best = it;
        for (auto jt = std::next(it); jt != last; ++jt) {
            if (compare(*jt, *best)) {
                best = jt;
            }
        }
        if (best != it) {
            std::iter_swap(it, best);
        }
    }
}

template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void insertionSort(RandomIt first, RandomIt last, Compare compare = Compare{}) {
    if (first == last) {
        return;
    }
    for (auto it = std::next(first); it != last; ++it) {
        auto value = std::move(*it);
        auto jt = it;
        while (jt != first && compare(value, *std::prev(jt))) {
            *jt = std::move(*std::prev(jt));
            --jt;
        }
        *jt = std::move(value);
    }
}

template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void heapSort(RandomIt first, RandomIt last, Compare compare = Compare{}) {
    using Diff = typename std::iterator_traits<RandomIt>::difference_type;
    Diff length = last - first;
    if (length < 2) {
        return;
    }

    auto sift_down = [&](Diff root, Diff end) -> void {
        while (true) {
            Diff child = root * 2 + 1;
            if (child > end) {
                break;
            }
            Diff best = root;
            if (compare(*(first + best), *(first + child))) {
                best = child;
            }
            if (child + 1 <= end && compare(*(first + best), *(first + (child + 1)))) {
                best = child + 1;
            }
            if (best == root) {
                return;
            }
            std::iter_swap(first + root, first + best);
            root = best;
        }
    };

    for (Diff start = length / 2; start-- > 0;) {
        sift_down(start, length - 1);
    }
    for (Diff end = length - 1; end > 0; --end) {
        std::iter_swap(first, first + end);
        sift_down(0, end - 1);
    }
}

template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void quickSort(RandomIt first, RandomIt last, Compare compare = Compare{}) {
    if (last - first < 2) {
        return;
    }

    auto pivot = *(first + (last - first) / 2);
    auto left = first;
    auto right = last - 1;

    while (left <= right) {
        while (compare(*left, pivot)) {
            ++left;
        }
        while (compare(pivot, *right)) {
            --right;
        }
        if (left <= right) {
            std::iter_swap(left, right);
            ++left;
            --right;
        }
    }

    if (first < right + 1) {
        quickSort(first, right + 1, compare);
    }
    if (left < last) {
        quickSort(left, last, compare);
    }
}

template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void mergeSort(RandomIt first, RandomIt last, Compare compare = Compare{}) {
    using Diff = typename std::iterator_traits<RandomIt>::difference_type;
    Diff length = last - first;
    if (length < 2) {
        return;
    }

    using Value = typename std::iterator_traits<RandomIt>::value_type;
    std::vector<Value> buffer(static_cast<std::size_t>(length));

    auto sort_impl = [&](auto&& self, Diff left, Diff right) -> void {
        if (left >= right) {
            return;
        }
        Diff mid = left + (right - left) / 2;
        self(self, left, mid);
        self(self, mid + 1, right);

        Diff i = left;
        Diff j = mid + 1;
        Diff k = left;

        while (i <= mid && j <= right) {
            if (compare(*(first + j), *(first + i))) {
                buffer[static_cast<std::size_t>(k++)] = std::move(*(first + j++));
            } else {
                buffer[static_cast<std::size_t>(k++)] = std::move(*(first + i++));
            }
        }
        while (i <= mid) {
            buffer[static_cast<std::size_t>(k++)] = std::move(*(first + i++));
        }
        while (j <= right) {
            buffer[static_cast<std::size_t>(k++)] = std::move(*(first + j++));
        }
        for (Diff index = left; index <= right; ++index) {
            *(first + index) = std::move(buffer[static_cast<std::size_t>(index)]);
        }
    };

    sort_impl(sort_impl, 0, length - 1);
}

template <typename InputIt, typename T>
InputIt linearSearch(InputIt first, InputIt last, const T& value) {
    for (; first != last; ++first) {
        if (*first == value) {
            return first;
        }
    }
    return last;
}

// Tìm kiếm nhị phân: trả về iterator trỏ đến phần tử nếu tìm thấy,
// hoặc `last` gốc nếu không tìm thấy.
// Yêu cầu: [first, last) đã được sắp xếp theo Compare.
template <typename RandomIt, typename T, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
RandomIt binarySearch(RandomIt first, RandomIt last, const T& value, Compare compare = Compare{}) {
    auto original_last = last;
    while (first < last) {
        auto mid = first + (last - first) / 2;
        if (compare(*mid, value)) {
            first = mid + 1;
        } else if (compare(value, *mid)) {
            last = mid;
        } else {
            return mid;
        }
    }
    return original_last;
}

} // namespace ds