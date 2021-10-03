// SPDX-License-Identifier: MIT
// Copyright (c) 2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include <scicpp/core.hpp>

int main() {
    using namespace scicpp::stats;
    const auto a = std::array{0, 0, 0, 1, 2, 3, 3, 4, 5};
    const auto [hist_a, bins_a] = histogram<BinEdgesMethod::AUTO>(a);
    scicpp::print(bins_a);
    scicpp::print(hist_a);

    const auto b = std::vector{0., 1.25, 2.5, 3.75, 5.};
    const auto [hist_b, bins_b] = histogram(b, 4);
    scicpp::print(bins_b);
    scicpp::print(hist_b);

    using namespace scicpp::units::literals;
    const auto c = std::array{0_V, 0_V, 0_V, 1_V, 2_V, 3_V, 3_V, 4_V, 5_V};
    const auto [hist_c, bins_c] = histogram<Density>(c);
    scicpp::print(bins_c);
    scicpp::print(hist_c);
}
