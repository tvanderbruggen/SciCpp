// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_IO
#define SCICPP_CORE_IO

#include "scicpp/core/macros.hpp"
#include "scicpp/core/meta.hpp"

#include <Eigen/Dense>
#include <any>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

namespace scicpp {

// A dictionary mapping column number to a function that will parse the
// column string into the desired value.

using ConvertersDict =
    typename std::map<signed_size_t, std::function<std::any(const char *str)>>;

namespace detail {

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

template <typename DataType>
int push_string_to_vector(std::vector<DataType> &vec,
                          const char *str,
                          char sep,
                          const ConvertersDict &converters = {}) {
    std::stringstream ss(str);
    std::string tmp;
    int len = 0;

    while (std::getline(ss, tmp, sep)) {
        if (!converters.empty()) {
            const auto converter = converters.find(len);

            if (converter != converters.end()) {
                vec.push_back(std::any_cast<DataType>(converter->second(tmp.data())));
            } else {
                vec.push_back(to_number<DataType>(tmp));
            }
        } else {
            vec.push_back(to_number<DataType>(tmp));
        }

        ++len;
    }

    return len;
}

auto tokenize(const char *str, char sep) {
    std::stringstream ss(str);
    std::string tmp;
    std::vector<std::string> tokens;

    while (std::getline(ss, tmp, sep)) {
        tokens.push_back(tmp);
    }

    return tokens;
}

template <typename DataType0, typename... DataTypes>
auto tokens_to_tuple(const std::vector<std::string> &tokens,
                     const ConvertersDict &converters = {}) {
    const auto idx = tokens.size() - sizeof...(DataTypes) - 1;
    DataType0 res;

    if (!converters.empty()) {
        const auto converter = converters.find(signed_size_t(idx));

        if (converter != converters.end()) {
            res =
                std::any_cast<DataType0>(converter->second(tokens[idx].data()));
        } else {
            res = to_number<DataType0>(tokens[idx]);
        }
    } else {
        res = to_number<DataType0>(tokens[idx]);
    }

    if constexpr (sizeof...(DataTypes) == 0) {
        return std::make_tuple(res);
    } else {
        return std::tuple_cat(
            std::make_tuple(res),
            tokens_to_tuple<DataTypes...>(tokens, converters));
    }
}

template <typename... DataTypes>
auto load_line_to_tuple(const char *str,
                        char sep,
                        const ConvertersDict &converters = {}) {
    const auto tokens = tokenize(str, sep);
    scicpp_require(tokens.size() == sizeof...(DataTypes));
    return tokens_to_tuple<DataTypes...>(tokens, converters);
}

} // namespace detail

//---------------------------------------------------------------------------------
// fromstring
//---------------------------------------------------------------------------------

template <typename DataType = double>
auto fromstring(const char *str,
                char sep,
                const ConvertersDict &converters = {}) {
    std::vector<DataType> res;
    detail::push_string_to_vector(res, str, sep, converters);
    return res;
}

template <typename DataType = double>
auto fromstring(const std::string &str,
                char sep,
                const ConvertersDict &converters = {}) {
    return fromstring(str.data(), sep, converters);
}

//---------------------------------------------------------------------------------
// loadtxt
//---------------------------------------------------------------------------------

// Load all the data in a single vector and return the number of columns.
template <typename DataType>
auto loadtxt_to_vector(const std::filesystem::path &fname,
                       char delimiter,
                       int skiprows,
                       const ConvertersDict &converters = {}) {
    std::vector<DataType> res(0);
    std::ifstream file(fname);
    int skip = skiprows;
    bool is_first_row = true;
    int num_cols = 0;

    if (file.is_open()) {
        std::string line;

        while (skip > 0) {
            --skip;
            std::getline(file, line);
        }

        while (std::getline(file, line)) {
            int line_cols = detail::push_string_to_vector(
                res, line.data(), delimiter, converters);

            if (is_first_row) {
                num_cols = line_cols;
            } else {
                scicpp_require(line_cols == num_cols);
            }

            is_first_row = false;
        }

        file.close();
    }

    return std::make_tuple(res, num_cols);
}

template <class EigenMatrix,
          std::enable_if_t<meta::is_eigen_matrix_v<EigenMatrix>, int> = 0>
EigenMatrix loadtxt(const std::filesystem::path &fname,
                    char delimiter,
                    int skiprows,
                    const ConvertersDict &converters = {}) {
    using T = typename EigenMatrix::value_type;

    const auto [data, num_cols] =
        loadtxt_to_vector<T>(fname, delimiter, skiprows, converters);

    return Eigen::Map<const Eigen::Matrix<typename EigenMatrix::Scalar,
                                          EigenMatrix::RowsAtCompileTime,
                                          EigenMatrix::ColsAtCompileTime,
                                          Eigen::RowMajor>>(
        data.data(), signed_size_t(data.size()) / num_cols, num_cols);
}

template <typename DataType = double,
          std::enable_if_t<std::is_arithmetic_v<DataType>, int> = 0>
auto loadtxt(const std::filesystem::path &fname,
             char delimiter,
             int skiprows,
             const ConvertersDict &converters = {}) {
    return loadtxt<Eigen::Matrix<DataType, Eigen::Dynamic, Eigen::Dynamic>>(
        fname, delimiter, skiprows, converters);
}

template <typename... DataTypes,
          std::enable_if_t<(sizeof...(DataTypes) > 1), int> = 0>
auto loadtxt(
    const std::filesystem::path &fname,
    char delimiter,
    int skiprows,
    [[maybe_unused]] const ConvertersDict &converters = {}) {
    std::vector<std::tuple<DataTypes...>> res;

    std::ifstream file(fname);
    int skip = skiprows;

    if (file.is_open()) {
        std::string line;

        while (skip > 0) {
            --skip;
            std::getline(file, line);
        }

        while (std::getline(file, line)) {
            res.push_back(detail::load_line_to_tuple<DataTypes...>(line.data(),
                                                                   delimiter));
        }

        file.close();
    }

    return res;
}

template <typename... DataTypes>
class TxtLoader {
  public:
    TxtLoader() {}

    auto delimiter(char delimiter_) {
        m_delimiter = delimiter_;
        return *this;
    }

    auto skiprows(int skiprows_) {
        m_skiprows = skiprows_;
        return *this;
    }

    auto converters(ConvertersDict converters_) {
        m_converters = converters_;
        return *this;
    }

    auto load(const std::filesystem::path &fname) {
        return loadtxt<DataTypes...>(
            fname, m_delimiter, m_skiprows, m_converters);
    }

  private:
    char m_delimiter = ' ';
    int m_skiprows = 0;
    ConvertersDict m_converters = {};
}; // class TxtLoader

} // namespace scicpp

#endif // SCICPP_CORE_IO