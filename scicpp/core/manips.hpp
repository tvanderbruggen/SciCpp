// SPDX-License-Identifier: MIT
// Copyright (c) 2024 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

// Array manipulation

#ifndef SCICPP_CORE_MANIPS
#define SCICPP_CORE_MANIPS

#include "scicpp/core/macros.hpp"
#include "scicpp/core/meta.hpp"
#include "scicpp/core/range.hpp"

#include <array>
#include <tuple>
#include <type_traits>
#include <vector>

namespace scicpp {

//-----------------------------------------------------------------------------
// concatenate
//-----------------------------------------------------------------------------

namespace detail {

template <typename Array1, typename Array2, typename ArrayOut>
void concatenate_impl(const Array1 &a1, const Array2 &a2, ArrayOut &res) {
    std::copy(a1.cbegin(), a1.cend(), res.begin());
    std::copy(a2.cbegin(), a2.cend(), res.begin() + signed_size_t(a1.size()));
}

} // namespace detail

template <typename T1, std::size_t N1, typename T2, std::size_t N2>
auto concatenate(const std::array<T1, N1> &a1, const std::array<T2, N2> &a2) {
    using T = std::common_type_t<T1, T2>;

    std::array<T, N1 + N2> res{};
    detail::concatenate_impl(a1, a2, res);
    return res;
}

template <typename Array, typename T2, meta::enable_if_iterable<Array> = 0>
auto concatenate(const Array &a1, const std::vector<T2> &a2) {
    using T1 = typename Array::value_type;
    using T = std::common_type_t<T1, T2>;

    std::vector<T> res(a1.size() + a2.size());
    detail::concatenate_impl(a1, a2, res);
    return res;
}

template <typename Array, typename T, meta::enable_if_iterable<Array> = 0>
auto concatenate(std::vector<T> &&a1, const Array &a2) {
    static_assert(std::is_convertible_v<T, typename Array::value_type>);

    const auto N1 = a1.size();
    a1.resize(N1 + a2.size());
    std::transform(a2.cbegin(),
                   a2.cend(),
                   a1.begin() + signed_size_t(N1),
                   [](auto x) { return static_cast<T>(x); });
    return std::move(a1);
}

template <typename Array, typename T, meta::enable_if_iterable<Array> = 0>
auto concatenate(const Array &a1, std::vector<T> &&a2) {
    static_assert(std::is_convertible_v<T, typename Array::value_type>);

    a2.insert(a2.begin(), a1.cbegin(), a1.cend());
    return std::move(a2);
}

namespace operators {

template <class ArrayLhs,
          class ArrayRhs,
          meta::enable_if_iterable<ArrayLhs> = 0,
          meta::enable_if_iterable<ArrayRhs> = 0>
auto operator|(ArrayLhs &&a, ArrayRhs &&b) {
    return concatenate(std::forward<ArrayLhs>(a), std::forward<ArrayRhs>(b));
}

} // namespace operators

template <typename Array0, typename... Arrays>
auto concatenate(const std::tuple<Array0, Arrays...> &tup) {
    using namespace operators;
    return std::apply([=](const auto &...a) { return (a | ...); }, tup);
}

template <typename... Arrays>
auto concatenate(const Arrays &...a) {
    return concatenate(std::tuple{a...});
}

} // namespace scicpp

#endif // SCICPP_CORE_MANIPS