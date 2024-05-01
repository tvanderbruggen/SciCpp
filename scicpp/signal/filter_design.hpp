// SPDX-License-Identifier: MIT
// Copyright (c) 2024 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_SIGNAL_FILTER_DESIGN
#define SCICPP_SIGNAL_FILTER_DESIGN

#include "scicpp/core/meta.hpp"

namespace scicpp::signal {

//---------------------------------------------------------------------------------
// unique_roots
//---------------------------------------------------------------------------------

enum UniqueRootsRtype : int { MAX, MIN, AVG };

template <typename Array, typename T>
auto unique_roots(const Array &p, T tol=1E-3, int rtype=MIN) {
    static_assert(meta::is_iterable_v<Array>);
}


} // namespace scicpp::signal

#endif // SCICPP_SIGNAL_FILTER_DESIGN