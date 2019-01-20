// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_LINALG_UTILS
#define SCICPP_LINALG_UTILS

#include <Eigen/Dense>
#include <array>
#include <vector>

namespace scicpp::linalg {

//---------------------------------------------------------------------------------
// Eigen types conversions
//---------------------------------------------------------------------------------

template <typename T>
auto to_eigen_matrix(const std::vector<T> &v, int size = -1) {
    if (size == -1) {
        size = int(v.size());
    }

    return Eigen::Map<const Eigen::Matrix<T, Eigen::Dynamic, 1>,
                      Eigen::Unaligned>(v.data(), size);
}

template <typename T>
auto to_eigen_array(const std::vector<T> &v, int size = -1) {
    if (size == -1) {
        size = int(v.size());
    }

    return Eigen::Map<const Eigen::Array<T, Eigen::Dynamic, 1>,
                      Eigen::Unaligned>(v.data(), size);
}

template <int size = -1, typename T, std::size_t N>
auto to_eigen_matrix(const std::array<T, N> &a) {
    constexpr int M = size == -1 ? int(N) : size;
    return Eigen::Matrix<T, M, 1>(a.data());
}

template <int size = -1, typename T, std::size_t N>
auto to_eigen_array(const std::array<T, N> &a) {
    constexpr int M = size == -1 ? int(N) : size;
    return Eigen::Array<T, M, 1>(a.data());
}

// Convert an Eigen matrix to a std container:
// - std::vector if dynamic size matrix
// - std::array if fixed size matrix
template <class EigenMatrix>
auto to_std_container(EigenMatrix m) {
    using T = typename EigenMatrix::value_type;
    constexpr int N = EigenMatrix::SizeAtCompileTime;

    if constexpr (N == Eigen::Dynamic) {
        return std::vector<T>(std::make_move_iterator(m.data()),
                              std::make_move_iterator(m.data() + m.size()));
    } else {
        std::array<T, std::size_t(N)> res{};
        std::move(m.data(), m.data() + N, res.begin());
        return res;
    }
}

} // namespace scicpp::linalg

#endif // SCICPP_LINALG_UTILS