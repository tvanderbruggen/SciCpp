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
//---------------------------------------------------------------------------------

namespace cst {

template <typename T>
constexpr auto c = units::speed<T>(2.99792458E8);

} // namespace cst

} // namespace scicpp

#endif // SCICPP_CORE_CONSTANTS