// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_RANGE
#define SCICPP_CORE_RANGE

#include <algorithm>
#include <array>
#include <cmath>
#include <type_traits>
#include <vector>

namespace scicpp {

//---------------------------------------------------------------------------------
// Linspace
//---------------------------------------------------------------------------------

namespace detail {

template <class Array, typename T = typename Array::value_type>
constexpr void linspace_filler(Array &arr, T start, T stop) {
    static_assert(std::is_floating_point<T>::value);

    if (arr.size() == 0) {
        return;
    }

    if (arr.size() == 1) {
        arr[0] = start;
        return;
    }

    const T step = (stop - start) / T(arr.size() - 1);

    for (std::size_t i = 0; i < arr.size(); ++i) {
        arr[i] = start + T(i) * step;
    }
}

} // namespace detail

template <std::size_t N, typename T>
constexpr auto linspace(T start, T stop) {
    std::array<T, N> arr{};
    detail::linspace_filler(arr, start, stop);
    return arr;
}

template <typename T>
auto linspace(T start, T stop, std::size_t num) {
    std::vector<T> vec(num);
    detail::linspace_filler(vec, start, stop);
    return vec;
}

//---------------------------------------------------------------------------------
// Logspace
//---------------------------------------------------------------------------------

namespace detail {

template <int Base, class Array, typename T = typename Array::value_type>
void logspace_filler(Array &arr, T start, T stop) {
    static_assert(std::is_floating_point_v<T>);

    if (arr.size() == 0) {
        return;
    }

    if (arr.size() == 1) {
        arr[0] = std::pow(T(Base), start);
        return;
    }

    const T step = (stop - start) / T(arr.size() - 1);

    std::generate(arr.begin(), arr.end(), [&, n = -1]() mutable {
        n++;
        return std::pow(T(Base), std::fma(T(n), step, start));
    });
}

} // namespace detail

template <std::size_t N, typename T, int Base = 10>
auto logspace(T start, T stop) {
    std::array<T, N> arr{};
    detail::logspace_filler<Base>(arr, start, stop);
    return arr;
}

template <typename T, int Base = 10>
auto logspace(T start, T stop, std::size_t num) {
    std::vector<T> vec(num);
    detail::logspace_filler<Base>(vec, start, stop);
    return vec;
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
