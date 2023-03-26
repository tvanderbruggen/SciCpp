// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2023 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_PLOTS_UTILS
#define SCICPP_PLOTS_UTILS

#include <unordered_map>

namespace scicpp::plots {

//---------------------------------------------------------------------------------
// Matplotlib markers to Gnuplot point types
//---------------------------------------------------------------------------------

using MarkersMap = std::unordered_map<char, int>;

static const MarkersMap markers = {{'.', 0},
                                   {'+', 1},
                                   {'x', 2},
                                   {'s', 5},
                                   {'o', 7},
                                   {'^', 9},
                                   {'v', 11},
                                   {'D', 13}};

int marker(char c) {
    return markers.find(c)->second;
}

} // namespace scicpp::plots

#endif // SCICPP_PLOTS_UTILS