// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_IO
#define SCICPP_CORE_IO

#include "scicpp/core/macros.hpp"
#include "scicpp/core/meta.hpp"

#include <Eigen/Dense>
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

namespace detail {

template <typename DataType0, typename... DataTypes>
struct ConvertersDict_ {
    using type = typename std::map<
        int,
        std::conditional_t<sizeof...(DataTypes) == 0,
                           std::function<DataType0(const char *str)>,
                           std::function<std::tuple<DataType0, DataTypes...>(
                               const char *str)>>>;
};

} // namespace detail

template <typename... DataTypes>
using ConvertersDict = typename detail::ConvertersDict_<DataTypes...>::type;

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
                          const ConvertersDict<DataType> &converters = {}) {
    std::stringstream ss(str);
    std::string tmp;
    int len = 0;

    while (std::getline(ss, tmp, sep)) {
        if (!converters.empty()) {
            const auto converter = converters.find(len);

            if (converter != converters.end()) {
                vec.push_back(converter->second(tmp.data()));
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
auto tokens_to_tuple(const std::vector<std::string> &tokens) {
    const auto idx = tokens.size() - sizeof...(DataTypes) - 1;

    if constexpr (sizeof...(DataTypes) == 0) {
        return std::make_tuple(to_number<DataType0>(tokens[idx]));
    } else {
        return std::tuple_cat(
            std::make_tuple(to_number<DataType0>(tokens[idx])),
            tokens_to_tuple<DataTypes...>(tokens));
    }
}

template <typename... DataTypes>
auto load_line_to_tuple(const char *str, char sep) {
    const auto tokens = tokenize(str, sep);
    scicpp_require(tokens.size() == sizeof...(DataTypes));
    return tokens_to_tuple<DataTypes...>(tokens);
}

} // namespace detail

//---------------------------------------------------------------------------------
// fromstring
//---------------------------------------------------------------------------------

template <typename DataType = double>
auto fromstring(const char *str,
                char sep,
                const ConvertersDict<DataType> &converters = {}) {
    std::vector<DataType> res;
    detail::push_string_to_vector(res, str, sep, converters);
    return res;
}

template <typename DataType = double>
auto fromstring(const std::string &str,
                char sep,
                const ConvertersDict<DataType> &converters = {}) {
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
                       const ConvertersDict<DataType> &converters = {}) {
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
          typename DataType = typename EigenMatrix::value_type,
          std::enable_if_t<meta::is_eigen_matrix_v<EigenMatrix>, int> = 0>
EigenMatrix loadtxt(const std::filesystem::path &fname,
                    char delimiter,
                    int skiprows,
                    const ConvertersDict<DataType> &converters = {}) {
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
             const ConvertersDict<DataType> &converters = {}) {
    return loadtxt<Eigen::Matrix<DataType, Eigen::Dynamic, Eigen::Dynamic>>(
        fname, delimiter, skiprows, converters);
}

template <typename... DataTypes,
          std::enable_if_t<(sizeof...(DataTypes) > 1), int> = 0>
auto loadtxt(
    const std::filesystem::path &fname,
    char delimiter,
    int skiprows,
    [[maybe_unused]] const ConvertersDict<DataTypes...> &converters = {}) {
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

    auto converters(ConvertersDict<DataTypes...> converters_) {
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
    ConvertersDict<DataTypes...> m_converters = {};
}; // class TxtLoader

} // namespace scicpp

#endif // SCICPP_CORE_IO