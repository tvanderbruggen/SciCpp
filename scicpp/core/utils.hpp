// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_UTILS
#define SCICPP_CORE_UTILS

#include <Eigen/Dense>
#include <array>
#include <vector>

namespace scicpp::utils {

//---------------------------------------------------------------------------------
// set_array: Set an array of same size than a given array
//---------------------------------------------------------------------------------

template <typename T, std::size_t N>
auto set_array(const std::array<T, N> & /* unused */) {
    return std::array<T, N>{};
}

template <typename T>
auto set_array(std::vector<T> v) {
    return std::vector<T>(v.size());
}

} // namespace scicpp::utils

#endif // SCICPP_CORE_UTILS