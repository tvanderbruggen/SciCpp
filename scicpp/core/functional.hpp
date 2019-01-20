// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_FUNCTIONAL
#define SCICPP_CORE_FUNCTIONAL

#include "scicpp/core/macros.hpp"
#include "scicpp/core/utils.hpp"

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <utility>

namespace scicpp {

//---------------------------------------------------------------------------------
// map
//---------------------------------------------------------------------------------

// Unary operations

template <class Array, class UnaryOp>
auto map(UnaryOp op, Array &&a) {
    std::transform(a.cbegin(), a.cend(), a.begin(), op);
    return a;
}

template <class Array, class UnaryOp>
auto map(UnaryOp op, const Array &a) {
    Array res(a);
    return map(op, std::move(res));
}

// Binary operations

template <class Array, class BinaryOp>
auto map(BinaryOp op, Array &&a1, const Array &a2) {
    SCICPP_REQUIRE(a1.size() == a2.size());
    std::transform(a1.cbegin(), a1.cend(), a2.cbegin(), a1.begin(), op);
    return a1;
}

template <class Array, class BinaryOp>
auto map(BinaryOp op, const Array &a1, Array &&a2) {
    SCICPP_REQUIRE(a1.size() == a2.size());
    std::transform(a1.cbegin(), a1.cend(), a2.cbegin(), a2.begin(), op);
    return a2;
}

template <class Array, class BinaryOp>
auto map(BinaryOp op, Array &&a1, Array &&a2) {
    return map(op, std::move(a1), a2);
}

template <class Array, class BinaryOp>
auto map(BinaryOp op, const Array &a1, const Array &a2) {
    auto res(a1);
    return map(op, std::move(res), a2);
}

//---------------------------------------------------------------------------------
// filter_reduce
//---------------------------------------------------------------------------------

template <class InputIt,
          class Predicate,
          class BinaryOp,
          typename T = typename std::iterator_traits<InputIt>::value_type>
constexpr T filter_reduce(
    InputIt first, InputIt last, BinaryOp op, T init, Predicate filter) {
    static_assert(std::is_integral_v<std::invoke_result_t<Predicate, T>>);

    T res = init;

    for (; first != last; ++first) {
        if (filter(*first)) {
            res = op(res, *first);
        }
    }

    return res;
}

} // namespace scicpp

#endif // SCICPP_CORE_FUNCTIONAL