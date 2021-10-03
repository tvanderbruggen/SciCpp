// SPDX-License-Identifier: MIT
// Copyright (c) 2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include <scicpp/core.hpp>

int main() {
    using namespace scicpp::stats;
    const auto a = std::array{0, 0, 0, 1, 2, 3, 3, 4, 5};
    const auto bins0 = histogram_bin_edges<BinEdgesMethod::AUTO>(a);
    scicpp::print(bins0);

    const auto b = std::vector{0., 1.25, 2.5, 3.75, 5.};
    const auto bins1 = histogram_bin_edges(b);
    scicpp::print(bins1);

    using namespace scicpp::units::literals;
    const auto c = std::array{0_V, 0_V, 0_V, 1_V, 2_V, 3_V, 3_V, 4_V, 5_V};
    const auto bins2 = histogram_bin_edges(c, 20);
    scicpp::print(bins2);
}
