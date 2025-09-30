// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "utilities.hpp"

#include <type_traits>

namespace scicpp::units {

TEST_CASE("strip_units_t") {
    static_assert(std::is_same_v<strip_units_t<float>, float>);
    static_assert(std::is_same_v<strip_units_t<length<int>>, int>);
    static_assert(std::is_same_v<strip_units_t<std::complex<length<double>>>,
                                 std::complex<double>>);
    static_assert(std::is_same_v<strip_units_t<std::vector<gram<long double>>>,
                                 std::vector<long double>>);
    static_assert(std::is_same_v<strip_units_t<std::array<watt<int>, 10>>,
                                 std::array<int, 10>>);
    static_assert(std::is_same_v<strip_units_t<std::span<watt<int>, 10>>,
                                 std::span<int, 10>>);
    static_assert(
        std::is_same_v<
            strip_units_t<std::tuple<gram<long double>,
                                     watt<int>,
                                     std::complex<length<double>>,
                                     std::pair<meter<float>, frequency<int>>,
                                     std::vector<pascal<long double>>,
                                     std::optional<std::array<volt<int>, 3>>>>,
            std::tuple<long double,
                       int,
                       std::complex<double>,
                       std::pair<float, int>,
                       std::vector<long double>,
                       std::optional<std::array<int, 3>>>>);

    static_assert(std::is_same_v<strip_units_t<std::span<const watt<int>, 10>>,
                                 std::span<const int, 10>>);
    static_assert(
        std::is_same_v<strip_units_t<std::variant<const watt<int>,
                                                  std::array<volt<int>, 3>>>,
                       std::variant<const int, std::array<int, 3>>>);
    static_assert(
        std::is_same_v<strip_units_t<std::tuple<const watt<int>,
                                                std::array<volt<int>, 3>>>,
                       std::tuple<const int, std::array<int, 3>>>);
}

} // namespace scicpp::units