// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_FUNCTIONAL
#define SCICPP_CORE_FUNCTIONAL

#include "scicpp/core/macros.hpp"

#include <algorithm>

namespace scicpp {

template <class Array, class UnaryOp>
auto map_inplace(UnaryOp op, Array &a) {
    std::transform(a.cbegin(), a.cend(), a.begin(), op);
}

template <class Array, class UnaryOp>
auto map(UnaryOp op, const Array &a) {
    Array res(a);
    map_inplace(op, res);
    return res;
}

template <class Array, class BinaryOp>
auto map(BinaryOp op, const Array &a1, const Array &a2) {
    SCICPP_REQUIRE(a1.size() == a2.size());

    Array res(a1);
    std::transform(a1.cbegin(), a1.cend(), a2.cbegin(), res.begin(), op);
    return res;
}

} // namespace scicpp

#endif // SCICPP_CORE_FUNCTIONAL