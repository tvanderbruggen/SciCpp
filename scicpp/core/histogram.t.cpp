// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "histogram.hpp"

#include "scicpp/core/print.hpp"
#include "scicpp/core/units/units.hpp"

#include <array>

namespace scicpp::stats {

TEST_CASE("histogram_bin_edges") {
    REQUIRE(almost_equal<2>(histogram_bin_edges<AUTO>(std::array<double, 0>{}),
                            {0., 1.}));
    REQUIRE(almost_equal<2>(histogram_bin_edges<AUTO>(std::vector<double>(0)),
                            {0., 1.}));

    const auto arr = std::array{0., 0., 0., 1., 2., 3., 3., 4., 5.};
    // print(histogram_bin_edges<FD>(arr));

    REQUIRE(almost_equal<2>(histogram_bin_edges<SQRT>(arr),
                            {0., 1. + 2. / 3., 10. / 3., 5.}));
    REQUIRE(almost_equal<2>(histogram_bin_edges<SCOTT>(arr), {0., 2.5, 5.}));
    REQUIRE(almost_equal<2>(histogram_bin_edges<RICE>(arr),
                            {0., 1., 2., 3., 4., 5.}));
    REQUIRE(almost_equal<2>(histogram_bin_edges<STURGES>(arr),
                            {0., 1., 2., 3., 4., 5.}));
    REQUIRE(almost_equal<2>(histogram_bin_edges<FD>(arr), {0., 2.5, 5.}));
    REQUIRE(almost_equal<2>(histogram_bin_edges<DOANE>(arr),
                            {0., 1., 2., 3., 4., 5.}));
    // print(histogram_bin_edges<DOANE>(std::vector{1., 1., 1.}));
    REQUIRE(almost_equal<2>(histogram_bin_edges<DOANE>(std::vector{1., 1., 1.}),
                            {0.5, 1.5}));
    REQUIRE(almost_equal<2>(histogram_bin_edges<AUTO>(std::vector{1., 1., 1.}),
                            {0.5, 1.5}));
    // print(detail::bin_width<AUTO>(arr));
    REQUIRE(almost_equal<2>(histogram_bin_edges<AUTO>(arr),
                            {0., 1., 2., 3., 4., 5.}));

    REQUIRE(almost_equal<2>(histogram_bin_edges(arr),
                            {0., 0.5, 1., 1.5, 2., 2.5, 3., 3.5, 4., 4.5, 5.}));
    REQUIRE(
        almost_equal<2>(histogram_bin_edges(std::array<double, 0>{}),
                        {0., 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.}));
}

TEST_CASE("histogram_bin_edges physical quantities") {
    using namespace units::literals;

    REQUIRE(almost_equal<2>(
        histogram_bin_edges<AUTO>(std::array<units::mass<double>, 0>{}),
        {0_kg, 1_kg}));

    const auto arr = std::array{0_m, 0_m, 1_m, 2_m, 3_m, 3_m, 4_m, 5_m};
    // print(histogram_bin_edges<SQRT>(arr));

    REQUIRE(almost_equal<2>(histogram_bin_edges<SQRT>(arr),
                            {0_m, 1_m + 2_m / 3., 10_m / 3., 5_m}));
    REQUIRE(
        almost_equal<2>(histogram_bin_edges<SCOTT>(arr), {0_m, 2.5_m, 5_m}));
    REQUIRE(almost_equal<2>(histogram_bin_edges<RICE>(arr),
                            {0_m, 1.25_m, 2.5_m, 3.75_m, 5_m}));
    REQUIRE(almost_equal<2>(histogram_bin_edges<STURGES>(arr),
                            {0_m, 1.25_m, 2.5_m, 3.75_m, 5_m}));
    REQUIRE(almost_equal<2>(histogram_bin_edges<FD>(arr), {0_m, 2.5_m, 5_m}));
    REQUIRE(almost_equal<2>(histogram_bin_edges<DOANE>(arr),
                            {0_m, 1_m, 2_m, 3_m, 4_m, 5_m}));
    REQUIRE(
        almost_equal<2>(histogram_bin_edges<DOANE>(std::vector{1_A, 1_A, 1_A}),
                        {0.5_A, 1.5_A}));
    REQUIRE(almost_equal<2>(
        histogram_bin_edges<AUTO>(std::vector{1_A, 1_A, 1_A}), {0.5_A, 1.5_A}));
    REQUIRE(almost_equal<2>(histogram_bin_edges<AUTO>(arr),
                            {0_m, 1.25_m, 2.5_m, 3.75_m, 5_m}));
    REQUIRE(almost_equal<2>(
        histogram_bin_edges(arr),
        {0_m, 0.5_m, 1_m, 1.5_m, 2_m, 2.5_m, 3_m, 3.5_m, 4_m, 4.5_m, 5_m}));
}

TEST_CASE("histogram") {
    using namespace operators;
    const auto a = std::array{0., 0., 0., 1., 2., 3., 3., 4., 5.};

    REQUIRE(histogram(a, empty<double>()).empty());
    // print(histogram(a, {0., 1., 2., 3., 4., 5.}));
    REQUIRE(histogram(a, {0., 1., 2., 3., 4., 5.}) ==
            std::vector<signed_size_t>({3, 1, 1, 2, 2}));
    // print(histogram(a, {0., 1., 3., 4.5, 5.}));
    REQUIRE(histogram(a, {0., 1., 3., 4.5, 5.}) ==
            std::vector<signed_size_t>({3, 2, 3, 1}));
    // print(histogram<true>(a, {0., 1., 2., 3., 4., 5.}));
    REQUIRE(histogram<true>(a, {0., 1., 2., 3., 4., 5.}) ==
            std::vector<signed_size_t>({3, 1, 1, 2, 2}));

    const auto b = std::vector{560.,
                               482.,
                               456.,
                               458.,
                               745.,
                               867.,
                               785.,
                               596.,
                               785.,
                               389.,
                               223.,
                               253.,
                               852.};
    // print(histogram<true>(b, {200., 400., 600., 800., 1000.}));
    REQUIRE(histogram<true>(b, {200., 400., 600., 800., 1000.}) ==
            std::vector<signed_size_t>({3, 5, 3, 2}));
    REQUIRE(histogram(b, {200., 400., 600., 800., 1000.}) ==
            std::vector<signed_size_t>({3, 5, 3, 2}));
}

} // namespace scicpp::stats