// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_FUNCTIONAL
#define SCICPP_CORE_FUNCTIONAL

#include "scicpp/core/macros.hpp"
#include "scicpp/core/utils.hpp"

#include <algorithm>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

namespace scicpp {

//---------------------------------------------------------------------------------
// map
//---------------------------------------------------------------------------------

// Unary operations

// Using map for unary operation:
// https://godbolt.org/z/hsRapR
// Generates quasi-identical assembly as raw loop
// https://godbolt.org/z/jFnoA2

template <class Array, class UnaryOp>
[[nodiscard]] auto map(UnaryOp op, Array &&a) {
    using InputType = typename Array::value_type;
    using ReturnType = typename std::invoke_result_t<UnaryOp, InputType>;

    if constexpr (std::is_same_v<InputType, ReturnType>) {
        std::transform(a.cbegin(), a.cend(), a.begin(), op);
        return std::move(a);
    } else {

        // If InputType and ReturnType are different,
        // we cannot move and we need to create an array
        // of ReturnType elements to store the results.

        // Maybe if sizeof(InputType) > sizeof(OutputType)
        // we can steal the input memory ? Is that safe ?

        auto res = utils::set_array<ReturnType>(a);
        std::transform(a.cbegin(), a.cend(), res.begin(), op);
        return res;
    }
}

template <class Array, class UnaryOp>
[[nodiscard]] auto map(UnaryOp op, const Array &a) {
    using InputType = typename Array::value_type;
    using ReturnType = typename std::invoke_result_t<UnaryOp, InputType>;

    auto res = utils::set_array<ReturnType>(a);
    std::transform(a.cbegin(), a.cend(), res.begin(), op);
    return res;
}

// Binary operations

// TODO Binary operations should accept two arrays of different types
// ex. an array of real numbers and one of complex numbers.

template <class Array, class BinaryOp>
[[nodiscard]] auto map(BinaryOp op, Array &&a1, const Array &a2) {
    using InputType = typename Array::value_type;
    using ReturnType = typename std::invoke_result_t<BinaryOp, InputType, InputType>;

    SCICPP_REQUIRE(a1.size() == a2.size());

    if constexpr (std::is_same_v<InputType, ReturnType>) {
        std::transform(a1.cbegin(), a1.cend(), a2.cbegin(), a1.begin(), op);
        return std::move(a1);
    } else {
        auto res = utils::set_array<ReturnType>(a1);
        std::transform(a1.cbegin(), a1.cend(), a2.cbegin(), res.begin(), op);
        return res;
    }
}

template <class Array, class BinaryOp>
[[nodiscard]] auto map(BinaryOp op, const Array &a1, Array &&a2) {
    using InputType = typename Array::value_type;
    using ReturnType = typename std::invoke_result_t<BinaryOp, InputType, InputType>;

    SCICPP_REQUIRE(a1.size() == a2.size());

    if constexpr (std::is_same_v<InputType, ReturnType>) {
        std::transform(a1.cbegin(), a1.cend(), a2.cbegin(), a2.begin(), op);
        return std::move(a2);
    } else {
        auto res = utils::set_array<ReturnType>(a2);
        std::transform(a1.cbegin(), a1.cend(), a2.cbegin(), res.begin(), op);
        return res;
    }
}

template <class Array, class BinaryOp>
[[nodiscard]] auto map(BinaryOp op, Array &&a1, Array &&a2) {
    return map(op, std::move(a1), a2);
}

template <class Array, class BinaryOp>
[[nodiscard]] auto map(BinaryOp op, const Array &a1, const Array &a2) {
    auto res(a1);
    return map(op, std::move(res), a2);
}

//---------------------------------------------------------------------------------
// vectorize
//---------------------------------------------------------------------------------

// Ex. compute cos(sin(x)), where x is a vector:
// => Using vectorized functions
//    https://godbolt.org/z/HE8A12
// => Using raw loop:
//    https://godbolt.org/z/HUzgjH
//
// Both codes generate a single call to new, so vectorize don't produce
// unecessary temporaries.
// For the vectorized version first the sin loop is called, then the cos one.
// For the raw loop version a single loop call sin and cos.

template <class Func>
auto vectorize(Func &&f) {
    return [&](auto &&... arrays) {
        return map(
            [&](auto &&... args) {
                return std::invoke(std::forward<Func>(f),
                                   std::forward<decltype(args)>(args)...);
            },
            std::forward<decltype(arrays)>(arrays)...);
    };
}

//---------------------------------------------------------------------------------
// filter
//---------------------------------------------------------------------------------

// filter does resize the array in a way that depends on runtime arguments
// so we cannot implement it for std::array.

template <typename T, class UnaryPredicate>
[[nodiscard]] auto filter(std::vector<T> &&a, UnaryPredicate p) {
    static_assert(std::is_integral_v<std::invoke_result_t<UnaryPredicate, T>>);

    const auto i =
        std::remove_if(a.begin(), a.end(), [p](auto v) { return !p(v); });
    a.erase(i, a.end());
    return a;
}

template <typename T, class UnaryPredicate>
[[nodiscard]] auto filter(const std::vector<T> &a, UnaryPredicate p) {
    std::vector<T> res(a);
    return filter(std::move(res), p);
}

//---------------------------------------------------------------------------------
// filter_reduce
//---------------------------------------------------------------------------------

template <class InputIt,
          class UnaryPredicate,
          class BinaryOp,
          typename T = typename std::iterator_traits<InputIt>::value_type>
[[nodiscard]] constexpr scicpp_pure T filter_reduce(
    InputIt first, InputIt last, BinaryOp op, T init, UnaryPredicate filter) {
    static_assert(std::is_integral_v<std::invoke_result_t<UnaryPredicate, T>>);

    T res = init;

    for (; first != last; ++first) {
        if (filter(*first)) {
            res = op(res, *first);
        }
    }

    return res;
}

template <class Array,
          class UnaryPredicate,
          class BinaryOp,
          typename T = typename Array::value_type>
[[nodiscard]] constexpr scicpp_pure T
filter_reduce(const Array &a, BinaryOp op, T init, UnaryPredicate filter) {
    return filter_reduce(a.cbegin(), a.cend(), op, init, filter);
}

} // namespace scicpp

#endif // SCICPP_CORE_FUNCTIONAL