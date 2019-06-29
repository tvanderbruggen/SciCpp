// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_IO
#define SCICPP_CORE_IO

#include "scicpp/core/macros.hpp"
#include "scicpp/core/meta.hpp"

#include <Eigen/Dense>
#include <algorithm>
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
#include <utility>
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
        return static_cast<T>(std::atoi(str));
    }
}

template <typename T>
auto to_number(const std::string &str) {
    return to_number<T>(str.data());
}

template <typename DataType>
auto convert(const std::string &str,
             signed_size_t idx,
             const ConvertersDict &converters) {
    if (!converters.empty()) {
        const auto converter = converters.find(idx);

        if (converter != converters.end()) {
            return std::any_cast<DataType>(converter->second(str.data()));
        } else {
            return to_number<DataType>(str);
        }
    } else {
        return to_number<DataType>(str);
    }
}

inline bool is_used_col(const std::vector<signed_size_t> &usecols,
                        signed_size_t col_idx) {
    if (usecols.empty()) {
        return true;
    } else {
        return std::find(usecols.cbegin(), usecols.cend(), col_idx) !=
               usecols.cend();
    }
}

template <typename DataType>
auto push_string_to_vector(std::vector<DataType> &vec,
                           const char *str,
                           char sep,
                           const ConvertersDict &converters,
                           const std::vector<signed_size_t> &usecols = {}) {
    std::stringstream ss(str);
    std::string tmp;
    signed_size_t len = 0;
    signed_size_t col_idx = 0;

    while (std::getline(ss, tmp, sep)) {
        if (is_used_col(usecols, col_idx)) {
            vec.push_back(convert<DataType>(tmp, len, converters));
            ++len;
        }

        ++col_idx;
    }

    return len;
}

auto inline tokenize(const char *str,
                     char sep,
                     const std::vector<signed_size_t> &usecols) {
    std::stringstream ss(str);
    std::string tmp;
    std::vector<std::pair<signed_size_t, std::string>> tokens(0);
    signed_size_t idx = 0;

    while (std::getline(ss, tmp, sep)) {
        if (is_used_col(usecols, idx)) {
            tokens.push_back(std::make_pair(idx, tmp));
        }

        ++idx;
    }

    return tokens;
}

template <typename DataType0, typename... DataTypes>
auto tokens_to_tuple(
    const std::vector<std::pair<signed_size_t, std::string>> &tokens,
    const ConvertersDict &converters) {
    const auto idx = tokens.size() - sizeof...(DataTypes) - 1;
    const auto res = convert<DataType0>(
        tokens[idx].second, signed_size_t(tokens[idx].first), converters);

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
                        const ConvertersDict &converters,
                        const std::vector<signed_size_t> &usecols) {
    const auto tokens = tokenize(str, sep, usecols);
    scicpp_require(tokens.size() == sizeof...(DataTypes));
    return tokens_to_tuple<DataTypes...>(tokens, converters);
}

template <class LineOp>
void iterate_file(const std::filesystem::path &fname, int skiprows, LineOp op) {
    std::ifstream file(fname);
    int skip = skiprows;

    if (file.is_open()) {
        std::string line;

        while (skip > 0) {
            --skip;
            std::getline(file, line);
        }

        while (std::getline(file, line)) {
            op(line);
        }

        file.close();
    }
}

// Load all the data in a single vector and return the number of columns.
template <typename DataType>
auto loadtxt_to_vector(const std::filesystem::path &fname,
                       char delimiter,
                       int skiprows,
                       const std::vector<signed_size_t> &usecols,
                       const ConvertersDict &converters) {
    std::vector<DataType> res(0);
    bool is_first_row = true;
    signed_size_t num_cols = 0;

    iterate_file(fname, skiprows, [&](auto line) {
        signed_size_t line_cols = detail::push_string_to_vector(
            res, line.data(), delimiter, converters, usecols);

        if (is_first_row) {
            num_cols = line_cols;
        } else {
            scicpp_require(line_cols == num_cols);
        }

        is_first_row = false;
    });

    return std::make_tuple(res, num_cols);
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

template <class EigenMatrix,
          std::enable_if_t<meta::is_eigen_matrix_v<EigenMatrix>, int> = 0>
EigenMatrix loadtxt(const std::filesystem::path &fname,
                    char delimiter,
                    int skiprows,
                    const std::vector<signed_size_t> &usecols,
                    const ConvertersDict &converters) {
    using T = typename EigenMatrix::value_type;

    const auto [data, num_cols] = detail::loadtxt_to_vector<T>(
        fname, delimiter, skiprows, usecols, converters);

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
             const std::vector<signed_size_t> &usecols,
             const ConvertersDict &converters) {
    return loadtxt<Eigen::Matrix<DataType, Eigen::Dynamic, Eigen::Dynamic>>(
        fname, delimiter, skiprows, usecols, converters);
}

template <typename... DataTypes,
          std::enable_if_t<(sizeof...(DataTypes) > 1), int> = 0>
auto loadtxt(const std::filesystem::path &fname,
             char delimiter,
             int skiprows,
             const std::vector<signed_size_t> &usecols,
             const ConvertersDict &converters) {
    std::vector<std::tuple<DataTypes...>> res;

    detail::iterate_file(fname, skiprows, [&](auto line) {
        res.push_back(detail::load_line_to_tuple<DataTypes...>(
            line.data(), delimiter, converters, usecols));
    });

    return res;
}

template <typename... DataTypes>
class TxtLoader {
  public:
    TxtLoader() = default;

    auto delimiter(char delimiter) {
        m_delimiter = delimiter;
        return *this;
    }

    auto skiprows(int skiprows) {
        m_skiprows = skiprows;
        return *this;
    }

    auto usecols(const std::vector<signed_size_t> &usecols) {
        m_usecols = usecols;
        return *this;
    }

    auto usecols(std::vector<signed_size_t> &&usecols) {
        m_usecols = std::move(usecols);
        return *this;
    }

    auto converters(ConvertersDict converters) {
        m_converters = converters;
        return *this;
    }

    auto load(const std::filesystem::path &fname) {
        return loadtxt<DataTypes...>(
            fname, m_delimiter, m_skiprows, m_usecols, m_converters);
    }

  private:
    char m_delimiter = ' ';
    int m_skiprows = 0;
    std::vector<signed_size_t> m_usecols = {};
    ConvertersDict m_converters = {};
}; // class TxtLoader

} // namespace scicpp

#endif // SCICPP_CORE_IO