// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_UTILS
#define SCICPP_CORE_UTILS

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

} // namespace scicpp::utils

#endif // SCICPP_CORE_UTILS