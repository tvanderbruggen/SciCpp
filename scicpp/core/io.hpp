// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_IO
#define SCICPP_CORE_IO

#include "scicpp/core/macros.hpp"
#include "scicpp/core/meta.hpp"
#include "scicpp/core/tuple.hpp"
#include "scicpp/core/units.hpp"

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
    std::map<signed_size_t, std::function<std::any(const char *str)>>;

namespace detail {

template <typename T>
auto to_number([[maybe_unused]] const char *str) {
    // Should use from_chars when implemented
    // https://en.cppreference.com/w/cpp/utility/from_chars
    if constexpr (units::is_quantity_v<T>) {
        return T(to_number<typename T::value_type>(str));
    } else if constexpr (std::is_floating_point_v<T>) {
        return static_cast<T>(std::atof(str));
    } else if constexpr (std::is_integral_v<T>) {
        return static_cast<T>(std::atoi(str));
    } else {
        return T{};
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

template <class TokenOp>
inline void iterate_line(const char *str,
                         char sep,
                         const std::vector<signed_size_t> &usecols,
                         TokenOp op) {
    std::stringstream ss(str);
    std::string tok;
    signed_size_t col_idx = 0;
    std::size_t usecol_idx = 0;

    while (std::getline(ss, tok, sep)) {
        if (!tok.empty()) {
            if (usecols.empty() || usecols[usecol_idx] == col_idx) {
                op(tok, col_idx);
                ++usecol_idx;

                if (usecol_idx == usecols.size()) {
                    break;
                }
            }

            ++col_idx;
        }
    }
}

template <typename DataType>
auto push_string_to_vector(std::vector<DataType> &vec,
                           const char *str,
                           char sep,
                           const ConvertersDict &converters,
                           const std::vector<signed_size_t> &usecols = {}) {
    signed_size_t len = 0;

    iterate_line(
        str, sep, usecols, [&](const auto &token, [[maybe_unused]] auto idx) {
            vec.push_back(convert<DataType>(token, len, converters));
            ++len;
        });

    return len;
}

template <class tokens_t>
auto tokenize(tokens_t &tokens,
              const char *str,
              char sep,
              const std::vector<signed_size_t> &usecols) {

    std::size_t tok_idx = 0;

    iterate_line(str, sep, usecols, [&](const auto &token, auto idx) {
        scicpp_require(tok_idx < tokens.size()); // Too many columns
        tokens[tok_idx] = std::make_pair(idx, token);
        ++tok_idx;
    });

    scicpp_require(tok_idx == tokens.size()); // Too few columns
    return tokens;
}

template <class tokens_t, typename DataType0, typename... DataTypes>
auto tokens_to_tuple(const tokens_t &tokens, const ConvertersDict &converters) {
    const auto idx = tokens.size() - sizeof...(DataTypes) - 1;
    const auto [tok_idx, tok_val] = tokens[idx];
    const auto res = convert<DataType0>(tok_val, tok_idx, converters);

    if constexpr (sizeof...(DataTypes) == 0) {
        return std::make_tuple(res);
    } else {
        return std::tuple_cat(
            std::make_tuple(res),
            tokens_to_tuple<tokens_t, DataTypes...>(tokens, converters));
    }
}

template <typename... DataTypes>
auto load_line_to_tuple(const char *str,
                        char sep,
                        const ConvertersDict &converters,
                        const std::vector<signed_size_t> &usecols) {
    using tokens_t =
        std::array<std::pair<signed_size_t, std::string>, sizeof...(DataTypes)>;

    tokens_t tokens{};
    tokenize(tokens, str, sep, usecols);
    return tokens_to_tuple<tokens_t, DataTypes...>(tokens, converters);
}

template <class LineOp>
void iterate_file(const std::filesystem::path &fname,
                  char comments,
                  signed_size_t skiprows,
                  signed_size_t max_rows,
                  LineOp op) {
    std::ifstream file(fname);
    signed_size_t skip = skiprows;

    if (file.is_open()) {
        std::string line;

        while (skip > 0) {
            --skip;
            std::getline(file, line);
        }

        while (std::getline(file, line) && max_rows != 0) {
            if (line[0] != comments) {
                op(line);
                --max_rows;
            }
        }

        file.close();
    }
}

// Load all the data in a single vector and return the number of columns.
template <typename DataType>
auto loadtxt_to_vector(const std::filesystem::path &fname,
                       char comments,
                       char delimiter,
                       signed_size_t skiprows,
                       const std::vector<signed_size_t> &usecols,
                       const ConvertersDict &converters,
                       signed_size_t max_rows) {
    std::vector<DataType> res(0);
    bool is_first_row = true;
    signed_size_t num_cols = 0;

    iterate_file(fname, comments, skiprows, max_rows, [&](auto line) {
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
                    char comments,
                    char delimiter,
                    signed_size_t skiprows,
                    const std::vector<signed_size_t> &usecols,
                    const ConvertersDict &converters,
                    signed_size_t max_rows) {
    using T = typename EigenMatrix::value_type;

    const auto [data, num_cols] = detail::loadtxt_to_vector<T>(
        fname, comments, delimiter, skiprows, usecols, converters, max_rows);

    return Eigen::Map<const Eigen::Matrix<typename EigenMatrix::Scalar,
                                          EigenMatrix::RowsAtCompileTime,
                                          EigenMatrix::ColsAtCompileTime,
                                          Eigen::RowMajor>>(
        data.data(), signed_size_t(data.size()) / num_cols, num_cols);
}

template <typename DataType = double,
          std::enable_if_t<std::is_arithmetic_v<DataType>, int> = 0>
auto loadtxt(const std::filesystem::path &fname,
             char comments,
             char delimiter,
             signed_size_t skiprows,
             const std::vector<signed_size_t> &usecols,
             const ConvertersDict &converters,
             signed_size_t max_rows) {
    return loadtxt<Eigen::Matrix<DataType, Eigen::Dynamic, Eigen::Dynamic>>(
        fname, comments, delimiter, skiprows, usecols, converters, max_rows);
}

template <typename... DataTypes,
          std::enable_if_t<(sizeof...(DataTypes) > 1), int> = 0>
auto loadtxt(const std::filesystem::path &fname,
             char comments,
             char delimiter,
             signed_size_t skiprows,
             const std::vector<signed_size_t> &usecols,
             const ConvertersDict &converters,
             signed_size_t max_rows) {
    std::vector<std::tuple<DataTypes...>> res;

    detail::iterate_file(fname, comments, skiprows, max_rows, [&](auto line) {
        res.push_back(detail::load_line_to_tuple<DataTypes...>(
            line.data(), delimiter, converters, usecols));
    });

    return res;
}

//---------------------------------------------------------------------------------
// TxtLoader
//
// A more versatile API to call loadtxt with default parameter values
// and explicit parameters naming.
//---------------------------------------------------------------------------------

namespace io {

inline constexpr bool unpack = true;

} // namespace io

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

    auto comments(char comments) {
        m_comments = comments;
        return *this;
    }

    auto usecols(const std::vector<signed_size_t> &usecols) {
        m_usecols = usecols;
        return *this;
    }

    auto usecols(std::vector<signed_size_t> &&usecols) {
        scicpp_require((sizeof...(DataTypes) > 1 &&
                        usecols.size() == sizeof...(DataTypes)) ||
                       (sizeof...(DataTypes) == 1));
        m_usecols = std::move(usecols);
        std::sort(m_usecols.begin(), m_usecols.end());
        return *this;
    }

    template <typename... Columns>
    auto usecols(Columns... usecols) {
        static_assert((std::is_integral_v<Columns> && ...),
                      "Used columns must be specified as an integer");
        static_assert(
            (sizeof...(DataTypes) > 1 &&
             sizeof...(Columns) == sizeof...(DataTypes)) ||
                (sizeof...(DataTypes) == 1),
            "Number of used columns must match the number of tuple elements");

        m_usecols.reserve(sizeof...(usecols));
        std::apply([this](auto... x) { (this->m_usecols.push_back(x), ...); },
                   std::forward_as_tuple(usecols...));
        std::sort(m_usecols.begin(), m_usecols.end());
        return *this;
    }

    auto converters(ConvertersDict converters) {
        m_converters = converters;
        return *this;
    }

    auto max_rows(signed_size_t max_rows) {
        m_max_rows = max_rows;
        return *this;
    }

    template <bool unpack = false>
    auto load(const std::filesystem::path &fname) {
        const auto data = loadtxt<DataTypes...>(fname,
                                                m_comments,
                                                m_delimiter,
                                                m_skiprows,
                                                m_usecols,
                                                m_converters,
                                                m_max_rows);

        if constexpr (unpack && (sizeof...(DataTypes) > 1)) {
            return scicpp::unpack(data);
        } else {
            return data;
        }
    }

  private:
    char m_delimiter = ' ';
    signed_size_t m_skiprows = 0;
    char m_comments = '#';
    std::vector<signed_size_t> m_usecols = {};
    ConvertersDict m_converters = {};
    signed_size_t m_max_rows = -1;
}; // class TxtLoader

} // namespace scicpp

#endif // SCICPP_CORE_IO