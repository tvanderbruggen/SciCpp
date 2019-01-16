// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_LINALG
#define SCICPP_CORE_LINALG

#include "scicpp/core/utils.hpp"

#include <Eigen/Dense>
#include <type_traits>

namespace scicpp::linalg {

template <class EigenMatrix, class StdContainer>
auto lstsq(const EigenMatrix &A, const StdContainer &b) {
    static_assert(std::is_same_v<typename EigenMatrix::value_type,
                                 typename StdContainer::value_type>);

    return utils::eigen::to_std_container(
        A.fullPivHouseholderQr()
            .solve(utils::eigen::to_eigen_matrix(b))
            .eval());
}

} // namespace scicpp::linalg

#endif // SCICPP_CORE_LINALG
