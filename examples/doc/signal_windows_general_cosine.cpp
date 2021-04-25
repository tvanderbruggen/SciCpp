// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include <array>
#include <scicpp/core.hpp>
#include <scicpp/signal.hpp>
#include <vector>

int main() {
    // Define the weights
    constexpr std::array HFT90D{1., 1.942604, 1.340318, 0.440811, 0.043097};

    // Window of 16 points stored in a std::array
    const auto window_array =
        scicpp::signal::windows::general_cosine<double, 16>(HFT90D);
    scicpp::print(window_array);

    // Window of 8192 points stored in a std::vector
    const auto window_vector =
        scicpp::signal::windows::general_cosine(8192, HFT90D);
    scicpp::print(window_vector);
}