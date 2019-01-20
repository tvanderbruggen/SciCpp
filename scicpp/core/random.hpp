// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_RANDOM
#define SCICPP_CORE_RANDOM

#include "scicpp/core/functional.hpp"

#include <array>
#include <random>
#include <type_traits>
#include <vector>

namespace scicpp::random {

namespace detail {

template <class Array, class RND>
void random_number_filler(Array &a, RND distribution) {
    static_assert(
        std::is_same_v<typename Array::value_type, typename RND::result_type>);

    std::mt19937 rng;
    map([&]([[maybe_unused]] auto v) { return distribution(rng); },
        std::move(a));
}

template <typename T>
const auto uniform_dist = std::uniform_real_distribution<T>{0., 1.};

template <typename T>
const auto normal_dist = std::normal_distribution<T>{0., 1.};

} // namespace detail

//---------------------------------------------------------------------------------
// rand
//---------------------------------------------------------------------------------

template <typename T, std::size_t N>
auto rand() {
    std::array<T, N> res{};
    detail::random_number_filler(res, detail::uniform_dist<T>);
    return res;
}

template <typename T>
T rand() {
    return rand<T, 1>()[0];
}

template <typename T>
auto rand(std::size_t N) {
    std::vector<T> res(N);
    detail::random_number_filler(res, detail::uniform_dist<T>);
    return res;
}

//---------------------------------------------------------------------------------
// randn
//---------------------------------------------------------------------------------

template <typename T, std::size_t N>
auto randn() {
    std::array<T, N> res{};
    detail::random_number_filler(res, detail::normal_dist<T>);
    return res;
}

template <typename T>
T randn() {
    return randn<T, 1>()[0];
}

template <typename T>
auto randn(std::size_t N) {
    std::vector<T> res(N);
    detail::random_number_filler(res, detail::normal_dist<T>);
    return res;
}

} // namespace scicpp::random

#endif // SCICPP_CORE_RANDOM