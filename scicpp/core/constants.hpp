// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_CONSTANTS
#define SCICPP_CORE_CONSTANTS

#include "scicpp/core/units.hpp"

namespace scicpp {

//---------------------------------------------------------------------------------
// Mathematical constants
//---------------------------------------------------------------------------------

template <typename T>
constexpr T pi = T(3.1415926535897932385);

//---------------------------------------------------------------------------------
// Physical constants
//
// From CODATA 2018
//---------------------------------------------------------------------------------

namespace cst {

using namespace units;

// Speed of light in vacuum
template <typename T = double>
constexpr auto c = speed<T>(299792458);

// Planck constant
template <typename T = double>
constexpr auto h = quantity_multiply<energy<T>, time<T>>(T(6.62607015E-34));

template <typename T = double>
constexpr auto hbar = h<T> / (T{2} * pi<T>);

// Elementaire charge
template <typename T = double>
constexpr auto e = charge<T>(T(1.602176634E-19));

} // namespace cst

} // namespace scicpp

#endif // SCICPP_CORE_CONSTANTS