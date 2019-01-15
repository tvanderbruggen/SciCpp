// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "utils.hpp"

#include "scicpp/core/numeric.hpp"

namespace scicpp::utils {

namespace eigen {

TEST_CASE("std::vector to Eigen::Matrix") {
    SECTION("Full size") {
        const std::vector v{1., 2., 3.};
        const auto m = to_eigen_matrix(v);
        REQUIRE(m.size() == v.size());
        Eigen::Matrix<double, Eigen::Dynamic, 1> m_(3);
        m_(0) = 1.;
        m_(1) = 2.;
        m_(2) = 3.;
        REQUIRE(m.isApprox(m_));
    }

    SECTION("Partial size") {
        const std::vector v{1., 2., 3., 4., 5.};
        const auto m = to_eigen_matrix(v, 3);
        REQUIRE(m.size() == 3);
        Eigen::Matrix<double, Eigen::Dynamic, 1> m_(3);
        m_(0) = 1.;
        m_(1) = 2.;
        m_(2) = 3.;
        REQUIRE(m.isApprox(m_));
    }
}

TEST_CASE("std::vector to Eigen::Array") {
    SECTION("Full size") {
        const std::vector v{1., 2., 3.};
        const auto m = to_eigen_array(v);
        REQUIRE(m.size() == v.size());
        Eigen::Array<double, Eigen::Dynamic, 1> m_(3);
        m_(0) = 1.;
        m_(1) = 2.;
        m_(2) = 3.;
        REQUIRE(m.isApprox(m_));
    }

    SECTION("Partial size") {
        const std::vector v{1., 2., 3., 4., 5.};
        const auto m = to_eigen_array(v, 3);
        REQUIRE(m.size() == 3);
        Eigen::Array<double, Eigen::Dynamic, 1> m_(3);
        m_(0) = 1.;
        m_(1) = 2.;
        m_(2) = 3.;
        REQUIRE(m.isApprox(m_));
    }
}

TEST_CASE("std::array to Eigen::Matrix") {
    SECTION("Full size") {
        const std::array v{1., 2., 3.};
        const auto m = to_eigen_matrix(v);
        REQUIRE(m.size() == v.size());
        Eigen::Matrix<double, Eigen::Dynamic, 1> m_(3);
        m_(0) = 1.;
        m_(1) = 2.;
        m_(2) = 3.;
        REQUIRE(m.isApprox(m_));
    }

    SECTION("Partial size") {
        const std::array v{1., 2., 3., 4., 5.};
        const auto m = to_eigen_matrix<3>(v);
        REQUIRE(m.size() == 3);
        Eigen::Matrix<double, Eigen::Dynamic, 1> m_(3);
        m_(0) = 1.;
        m_(1) = 2.;
        m_(2) = 3.;
        REQUIRE(m.isApprox(m_));
    }
}

TEST_CASE("std::array to Eigen::Array") {
    SECTION("Full size") {
        const std::array v{1., 2., 3.};
        const auto m = to_eigen_array(v);
        REQUIRE(m.size() == v.size());
        Eigen::Array<double, Eigen::Dynamic, 1> m_(3);
        m_(0) = 1.;
        m_(1) = 2.;
        m_(2) = 3.;
        REQUIRE(m.isApprox(m_));
    }

    SECTION("Partial size") {
        const std::array v{1., 2., 3., 4., 5.};
        const auto m = to_eigen_array<3>(v);
        REQUIRE(m.size() == 3);
        Eigen::Array<double, Eigen::Dynamic, 1> m_(3);
        m_(0) = 1.;
        m_(1) = 2.;
        m_(2) = 3.;
        REQUIRE(m.isApprox(m_));
    }
}

TEST_CASE("to_std_container") {
    SECTION("Dynamic size matrix") {
        Eigen::Matrix<double, Eigen::Dynamic, 1> m(3);
        m(0) = 1.;
        m(1) = 2.;
        m(2) = 3.;
        REQUIRE(almost_equal(to_std_container(m), {1., 2., 3.}));
    }

    SECTION("Fixed size matrix") {
        Eigen::Matrix<double, 3, 1> m;
        m << 1., 2., 3.;
        REQUIRE(almost_equal(to_std_container(m), {1., 2., 3.}));
    }
}

} // namespace eigen

} // namespace scicpp::utils