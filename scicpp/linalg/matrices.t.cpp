#include "matrices.hpp"

#include <vector>

namespace scicpp::linalg {

TEST_CASE("eye") {
    SECTION("dynamic size, m = 0") {
        const auto e = eye<double>(0);
        REQUIRE(e.rows() == 0);
        REQUIRE(e.rows() == 0);
    }

    SECTION("dynamic size, k = 0") {
        const auto e = eye<double>(3);
        REQUIRE(e.rows() == 3);
        REQUIRE(e.rows() == 3);
        Eigen::Matrix<double, 3, 3> m;
        m << 1., 0., 0., //
            0., 1., 0.,  //
            0., 0., 1.;  //
        // std::cout << m << std::endl;
        // std::cout << e << std::endl;
        REQUIRE(m.isApprox(e));
    }

    SECTION("dynamic size, k = -1") {
        const auto e = eye<double>(3, -1);
        REQUIRE(e.rows() == 3);
        REQUIRE(e.rows() == 3);
        Eigen::Matrix<double, 3, 3> m;
        m << 0., 0., 0., //
            1., 0., 0.,  //
            0., 1., 0.;  //
        // std::cout << m << std::endl;
        // std::cout << e << std::endl;
        REQUIRE(m.isApprox(e));
    }

    SECTION("dynamic size, k = 1") {
        const auto e = eye<double>(3, 1);
        REQUIRE(e.rows() == 3);
        REQUIRE(e.rows() == 3);
        Eigen::Matrix<double, 3, 3> m;
        m << 0., 1., 0., //
            0., 0., 1.,  //
            0., 0., 0.;  //
        // std::cout << m << std::endl;
        // std::cout << e << std::endl;
        REQUIRE(m.isApprox(e));
    }

    SECTION("fixed size, m = 0") {
        const auto e = eye<double, 0>();
        REQUIRE(e.rows() == 0);
        REQUIRE(e.rows() == 0);
    }

    SECTION("fixed size, k = 0") {
        const auto e = eye<double, 3>();
        REQUIRE(e.rows() == 3);
        REQUIRE(e.rows() == 3);
        Eigen::Matrix<double, 3, 3> m;
        m << 1., 0., 0., //
            0., 1., 0.,  //
            0., 0., 1.;  //
        // std::cout << m << std::endl;
        // std::cout << e << std::endl;
        REQUIRE(m.isApprox(e));
    }

    SECTION("fixed size, k = 1") {
        const auto e = eye<double, 3>(1);
        REQUIRE(e.rows() == 3);
        REQUIRE(e.rows() == 3);
        Eigen::Matrix<double, 3, 3> m;
        m << 0., 1., 0., //
            0., 0., 1.,  //
            0., 0., 0.;  //
        // std::cout << m << std::endl;
        // std::cout << e << std::endl;
        REQUIRE(m.isApprox(e));
    }
}

TEST_CASE("companion") {
    SECTION("vector 1") {
        const auto v = std::vector{4., 5., 6.};
        const auto c = companion(v);
        Eigen::Matrix<double, 2, 2> m;
        m << -1.25, -1.5, //
            1., 0.;       //
        // std::cout << c << std::endl;
        // std::cout << m << std::endl;
        REQUIRE(m.isApprox(c));
    }

    SECTION("vector 2") {
        const auto v = std::vector{-1., 5., 8., -10.};
        const auto c = companion(v);
        Eigen::Matrix<double, 3, 3> m;
        m << 5., 8., -10., //
            1., 0., 0.,    //
            0., 1., 0.;    //
        // std::cout << c << std::endl;
        // std::cout << m << std::endl;
        REQUIRE(m.isApprox(c));
    }

    SECTION("array 1") {
        const auto v = std::array{4., 5., 6.};
        const auto c = companion(v);
        Eigen::Matrix<double, 2, 2> m;
        m << -1.25, -1.5, //
            1., 0.;       //
        // std::cout << c << std::endl;
        // std::cout << m << std::endl;
        REQUIRE(m.isApprox(c));
    }
}

} // namespace scicpp::linalg