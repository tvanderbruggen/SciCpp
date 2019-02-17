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
#include <vector>

namespace scicpp {

//---------------------------------------------------------------------------------
// Linspace
//---------------------------------------------------------------------------------

namespace detail {

template <class Array, typename T = typename Array::value_type>
auto linspace_filler(Array &&a, T start, T stop) {
    using namespace scicpp::operators;

    if (a.size() == 0) {
        return std::move(a);
    }

    if (a.size() == 1) {
        a[0] = start;
        return std::move(a);
    }

    const T step = (stop - start) / T(a.size() - 1);
    std::iota(a.begin(), a.end(), T{0});
    return start + a * step;
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

template <class Array, typename T = typename Array::value_type>
auto logspace_filler(Array &&a, T start, T stop, T base) {
    if (a.size() == 0) {
        return std::move(a);
    }

    if (a.size() == 1) {
        a[0] = std::pow(base, start);
        return std::move(a);
    }

    const T step = (stop - start) / T(a.size() - 1);
    std::iota(a.begin(), a.end(), T{0});
    return map([=](auto x) { return std::pow(base, std::fma(x, step, start)); },
               a);
}

} // namespace detail

template <std::size_t N, typename T>
auto logspace(T start, T stop, T base = T{10}) {
    return detail::logspace_filler(std::array<T, N>{}, start, stop, base);
}

template <typename T>
auto logspace(T start, T stop, std::size_t num, T base = T{10}) {
    return detail::logspace_filler(std::vector<T>(num), start, stop, base);
}

//---------------------------------------------------------------------------------
// Arange
//---------------------------------------------------------------------------------

template <typename T>
auto arange(T start, T stop, T step = 1.0) {
    std::size_t num;

    if (((stop > start) && (step < 0.0)) || ((stop < start) && (step > 0.0))) {
        num = 0;
    } else {
        num = static_cast<std::size_t>(std::fabs((stop - start) / step));
    }

    std::vector<T> vec(num);

    if (num == 0) {
        return vec;
    }

    std::generate(vec.begin(), vec.end(), [&, n = -1]() mutable {
        n++;
        return std::fma(T(n), step, start);
    });

    return vec;
}

} // namespace scicpp

#endif // SCICPP_CORE_RANGE
