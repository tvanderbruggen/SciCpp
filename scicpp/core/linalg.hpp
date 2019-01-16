// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_LINALG
#define SCICPP_CORE_LINALG

#include "scicpp/core/utils.hpp"

#include <Eigen/Dense>
#include <array>
#include <vector>

namespace scicpp::linalg {

template <typename T, int M, int N>
auto lstsq(const Eigen::Matrix<T, M, N> &A,
           const std::array<T, std::size_t(M)> &b) {
    return utils::eigen::to_std_container(
        A.fullPivHouseholderQr()
            .solve(utils::eigen::to_eigen_matrix(b))
            .eval());
}

template <typename T>
auto lstsq(const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> &A,
           const std::vector<T> &b) {
    return utils::eigen::to_std_container(
        A.fullPivHouseholderQr()
            .solve(utils::eigen::to_eigen_matrix(b))
            .eval());
}

} // namespace scicpp::linalg

#endif // SCICPP_CORE_LINALG
