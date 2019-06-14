// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_LOADTXT
#define SCICPP_CORE_LOADTXT

#include "scicpp/core/macros.hpp"
#include "scicpp/core/meta.hpp"

#include <Eigen/Dense>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

namespace scicpp {

template <typename T>
auto to_number(const char *str) {
    if constexpr (std::is_floating_point_v<T>) {
        return std::atof(str);
    } else if constexpr (std::is_integral_v<T>) {
        return std::atoi(str);
    }
}

template <typename T>
auto to_number(const std::string &str) {
    return to_number<T>(str.data());
}

// Load all the data in a single vector and return the number of columns.
template <typename StdVector,
          std::enable_if_t<meta::is_std_vector_v<StdVector>, int> = 0>
auto loadtxt(const std::filesystem::path &fname, char delimiter, int skiprows) {
    using T = typename StdVector::value_type;

    StdVector res;
    std::ifstream file(fname);
    int skip = skiprows;
    bool is_first_row = true;
    int num_cols = 0;

    if (file.is_open()) {
        std::string line;
        std::string tmp;

        while (--skip) {
            std::getline(file, line);
        }

        while (std::getline(file, line)) {
            std::stringstream ss(line);

            while (std::getline(ss, tmp, delimiter)) {
                if (is_first_row) {
                    ++num_cols;
                }

                res.push_back(to_number<T>(tmp));
            }

            is_first_row = false;
        }

        file.close();
    }

    return std::make_tuple(res, num_cols);
}

template <class EigenMatrix,
          std::enable_if_t<meta::is_eigen_matrix_v<EigenMatrix>, int> = 0>
EigenMatrix
loadtxt(const std::filesystem::path &fname, char delimiter, int skiprows) {
    using T = typename EigenMatrix::value_type;

    const auto [data, num_cols] =
        loadtxt<std::vector<T>>(fname, delimiter, skiprows);

    return Eigen::Map<const Eigen::Matrix<typename EigenMatrix::Scalar,
                                          EigenMatrix::RowsAtCompileTime,
                                          EigenMatrix::ColsAtCompileTime,
                                          Eigen::RowMajor>>(
        data.data(), signed_size_t(data.size()) / num_cols, num_cols);
}

template <typename DataType = double,
          std::enable_if_t<std::is_arithmetic_v<DataType>, int> = 0>
auto loadtxt(const std::filesystem::path &fname, char delimiter, int skiprows) {
    return loadtxt<Eigen::Matrix<DataType, Eigen::Dynamic, Eigen::Dynamic>>(
        fname, delimiter, skiprows);
}

} // namespace scicpp

#endif // SCICPP_CORE_LOADTXT