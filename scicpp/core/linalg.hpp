// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_LINALG
#define SCICPP_CORE_LINALG

#include "scicpp/core/utils.hpp"

#include <Eigen/Dense>
#include <array>
#include <vector>

namespace scicpp {
namespace linalg {

template <typename T, int M, int N>
auto lstsq(const Eigen::Matrix<T, M, N> &A,
           const std::array<T, std::size_t(M)> &b) {
    Eigen::Matrix<T, N, 1> X =
        A.fullPivHouseholderQr().solve(utils::eigen::to_eigen_matrix(b));
    return utils::eigen::to_std_container(X);
}

template <typename T>
auto lstsq(const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> &A,
           const std::vector<T> &b) {
    Eigen::Matrix<T, Eigen::Dynamic, 1> X =
        A.fullPivHouseholderQr().solve(utils::eigen::to_eigen_matrix(b));
    return utils::eigen::to_std_container(X);
}

} // namespace linalg
} // namespace scicpp

#endif // SCICPP_CORE_LINALG
