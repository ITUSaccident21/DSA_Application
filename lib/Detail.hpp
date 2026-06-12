#pragma once

#include <utility>

namespace ds {

template <typename T>
struct Identity {
    constexpr const T& operator()(const T& value) const noexcept {
        return value;
    }
};

template <typename First, typename Second>
struct PairFirst {
    constexpr const First& operator()(const std::pair<First, Second>& value) const noexcept {
        return value.first;
    }
};

template <typename Compare, typename Lhs, typename Rhs>
constexpr bool compare_equivalent(const Compare& compare, const Lhs& lhs, const Rhs& rhs) {
    return !compare(lhs, rhs) && !compare(rhs, lhs);
}

} // namespace ds