// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_RANGE
#define SCICPP_CORE_RANGE

#include "scicpp/core/functional.hpp"
#include "scicpp/core/numeric.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>

namespace scicpp {

//---------------------------------------------------------------------------------
// Linspace
//---------------------------------------------------------------------------------

namespace detail {

template <class Array, typename T = typename Array::value_type>
auto linspace_filler(Array &&a, T start, T stop) {
    using namespace scicpp::operators;
    using raw_t = units::representation_t<T>;

    if (a.empty()) {
        return std::move(a);
    }

    if (a.size() == 1U) {
        a[0] = start;
        return std::move(a);
    }

    std::iota(a.begin(), a.end(), T(0));
    const auto step = units::value(stop - start) / raw_t(a.size() - 1);
    return start + std::move(a) * step;
}

} // namespace detail

template <std::size_t N, typename T>
auto linspace(T start, T stop) {
    return detail::linspace_filler(std::array<T, N>{}, start, stop);
}

template <typename T>
auto linspace(T start, T stop, std::size_t num) {
    return detail::linspace_filler(std::vector<T>(num), start, stop);
}

//---------------------------------------------------------------------------------
// Logspace
//---------------------------------------------------------------------------------

namespace detail {

template <class Array, typename BaseTp, typename T = typename Array::value_type>
auto logspace_filler(Array &&a, T start, T stop, BaseTp base) {
    if (a.size() == 0) {
        return std::move(a);
    }

    if (a.size() == 1) {
        a[0] = T(std::pow(base, units::value(start)));
        return std::move(a);
    }

    std::iota(a.begin(), a.end(), T(0));
    const auto step = (stop - start) / BaseTp(a.size() - 1);
    return map(
        [=](auto x) {
            return T(std::pow(base,
                              std::fma(units::value(x),
                                       units::value(step),
                                       units::value(start))));
        },
        a);
}

} // namespace detail

template <std::size_t N,
          typename T,
          typename BaseTp = units::representation_t<T>>
auto logspace(T start, T stop, BaseTp base = BaseTp{10}) {
    return detail::logspace_filler(std::array<T, N>{}, start, stop, base);
}

template <typename T, typename BaseTp = units::representation_t<T>>
auto logspace(T start, T stop, std::size_t num, BaseTp base = BaseTp{10}) {
    return detail::logspace_filler(std::vector<T>(num), start, stop, base);
}

//---------------------------------------------------------------------------------
// Arange
//---------------------------------------------------------------------------------

template <typename T>
auto arange(T start, T stop, T step = T{1}) {
    std::size_t num;

    if (((stop > start) && (step < T{0})) ||
        ((stop < start) && (step > T{0}))) {
        num = 0;
    } else {
        num = static_cast<std::size_t>(
            std::fabs(units::value(stop - start) / units::value(step)));
    }

    std::vector<T> v(num);
    std::iota(v.begin(), v.end(), T{0});
    return map(
        [=](auto x) {
            return T(std::fma(
                units::value(x), units::value(step), units::value(start)));
        },
        std::move(v));
}

} // namespace scicpp

#endif // SCICPP_CORE_RANGE
