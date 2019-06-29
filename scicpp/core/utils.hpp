// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_UTILS
#define SCICPP_CORE_UTILS

#include "scicpp/core/meta.hpp"

#include <algorithm>
#include <array>
#include <iterator>
#include <vector>

namespace scicpp::utils {

//---------------------------------------------------------------------------------
// set_array: Set an array of same size than a given array
//---------------------------------------------------------------------------------

template <typename OutputType, typename T, std::size_t N>
auto set_array(const std::array<T, N> & /* unused */) {
    return std::array<OutputType, N>{};
}

template <typename OutputType, typename T>
auto set_array(std::vector<T> v) {
    return std::vector<OutputType>(v.size());
}

template <class Array>
auto set_array(const Array &a) {
    return set_array<typename Array::value_type>(a);
}

//---------------------------------------------------------------------------------
// move_sub_vector
//---------------------------------------------------------------------------------

template <typename T>
auto move_subvector(std::vector<T> &&v, std::size_t len) {
    return std::vector<T>(
        std::make_move_iterator(v.begin()),
        std::make_move_iterator(v.begin() + int(std::min(len, v.size()))));
}

//---------------------------------------------------------------------------------
// get_elements: get tuple elements from an array of tuples
//---------------------------------------------------------------------------------

template <std::size_t I, typename T, std::size_t N>
auto get_elements(const std::array<T, N> &a) {
    static_assert(meta::is_std_tuple_v<T>);
    using elt_type = std::tuple_element_t<I, T>;

    auto res = std::array<elt_type, N>{};
    std::transform(a.cbegin(), a.cend(), res.begin(), [=](auto t) {
        return std::get<I>(t);
    });
    return res;
}

template <std::size_t I, typename T>
auto get_elements(const std::vector<T> &a) {
    static_assert(meta::is_std_tuple_v<T>);
    using elt_type = std::tuple_element_t<I, T>;

    auto res = std::vector<elt_type>(a.size());
    std::transform(a.cbegin(), a.cend(), res.begin(), [=](auto t) {
        return std::get<I>(t);
    });
    return res;
}

} // namespace scicpp::utils

#endif // SCICPP_CORE_UTILS