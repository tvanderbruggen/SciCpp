// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_FUNCTIONAL
#define SCICPP_CORE_FUNCTIONAL

#include "scicpp/core/macros.hpp"
#include "scicpp/core/utils.hpp"

#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <numeric>
#include <tuple>
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
    using InputType = typename std::remove_reference_t<Array>::value_type;
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
    using ReturnType =
        typename std::invoke_result_t<BinaryOp, InputType, InputType>;

    scicpp_require(a1.size() == a2.size());

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
    using ReturnType =
        typename std::invoke_result_t<BinaryOp, InputType, InputType>;

    scicpp_require(a1.size() == a2.size());

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
    return map(op, Array(a1), a2);
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
            [&](auto &&... args) scicpp_const {
                return std::invoke(std::forward<Func>(f),
                                   std::forward<decltype(args)>(args)...);
            },
            std::forward<decltype(arrays)>(arrays)...);
    };
}

//---------------------------------------------------------------------------------
// filter
//---------------------------------------------------------------------------------

namespace filters {

const auto all = []([[maybe_unused]] auto v) { return true; };
const auto none = []([[maybe_unused]] auto v) { return false; };
const auto not_nan = [](auto v) { return !std::isnan(v); };

} // namespace filters

// filter does resize the array in a way that depends on runtime arguments
// so we cannot implement it for std::array.

template <typename T, class UnaryPredicate>
[[nodiscard]] auto filter(std::vector<T> &&a, UnaryPredicate p) {
    static_assert(std::is_integral_v<std::invoke_result_t<UnaryPredicate, T>>);

    const auto i =
        std::remove_if(a.begin(), a.end(), [p](auto v) { return !p(v); });
    a.erase(i, a.end());
    return std::move(a);
}

template <typename T, class UnaryPredicate>
[[nodiscard]] auto filter(const std::vector<T> &a, UnaryPredicate p) {
    return filter(std::vector(a), p);
}

//---------------------------------------------------------------------------------
// filter_reduce
//---------------------------------------------------------------------------------

template <class InputIt,
          class UnaryPredicate,
          class BinaryOp,
          typename T = typename std::iterator_traits<InputIt>::value_type>
[[nodiscard]] constexpr scicpp_pure auto filter_reduce(
    InputIt first, InputIt last, BinaryOp op, T init, UnaryPredicate filter) {
    static_assert(std::is_integral_v<std::invoke_result_t<UnaryPredicate, T>>);

    T res = init;
    signed_size_t cnt = 0;

    for (; first != last; ++first) {
        if (filter(*first)) {
            res = op(res, *first);
            cnt++;
        }
    }

    return std::make_tuple(res, cnt);
}

template <class Array,
          class UnaryPredicate,
          class BinaryOp,
          typename T = typename Array::value_type>
[[nodiscard]] constexpr scicpp_pure auto
filter_reduce(const Array &a, BinaryOp op, T init, UnaryPredicate filter) {
    return filter_reduce(a.cbegin(), a.cend(), op, init, filter);
}

//---------------------------------------------------------------------------------
// filter_reduce_associative
//
// Use pairwise recursion for improved precision in associative operations.
//
// https://en.wikipedia.org/wiki/Pairwise_summation
// https://github.com/numpy/numpy/pull/3685
// https://github.com/JuliaLang/julia/pull/4039
//---------------------------------------------------------------------------------

template <class InputIt,
          class UnaryPredicate,
          class AssociativeBinaryOp,
          typename T = typename std::iterator_traits<InputIt>::value_type>
[[nodiscard]] constexpr scicpp_pure auto
filter_reduce_associative(InputIt first,
                          InputIt last,
                          AssociativeBinaryOp op,
                          T init,
                          UnaryPredicate filter) {
    constexpr long PW_BLOCKSIZE = 64;
    const auto size = std::distance(first, last);

    if (size <= PW_BLOCKSIZE) {
        return filter_reduce(first, last, op, init, filter);
    } else {
        const auto [res1, cnt1] = filter_reduce_associative(
            first, first + size / 2, op, init, filter);
        const auto [res2, cnt2] =
            filter_reduce_associative(first + size / 2, last, op, init, filter);
        return std::make_tuple(res1 + res2, cnt1 + cnt2);
    }
}

//---------------------------------------------------------------------------------
// reduce
//---------------------------------------------------------------------------------

template <class Array, class BinaryOp, typename T = typename Array::value_type>
[[nodiscard]] constexpr scicpp_pure auto
reduce(const Array &a, BinaryOp op, T init) {
    return filter_reduce(a, op, init, filters::all);
}

//---------------------------------------------------------------------------------
// cumacc
//---------------------------------------------------------------------------------

template <class Array, class BinaryOp, class UnaryPredicate>
auto cumacc(Array &&a, BinaryOp op, UnaryPredicate p) {
    using InputType = typename std::remove_reference_t<Array>::value_type;
    using ReturnType =
        typename std::invoke_result_t<BinaryOp, InputType, InputType>;
    static_assert(
        std::is_integral_v<std::invoke_result_t<UnaryPredicate, InputType>>);
    static_assert(std::is_same_v<InputType, ReturnType>);

    auto a_filt = filter(std::forward<Array>(a), p);
    std::partial_sum(a_filt.cbegin(), a_filt.cend(), a_filt.begin(), op);
    return a_filt;
}

} // namespace scicpp

#endif // SCICPP_CORE_FUNCTIONAL