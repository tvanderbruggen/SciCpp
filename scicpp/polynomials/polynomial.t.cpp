// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "polynomial.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/print.hpp"
#include "scicpp/core/random.hpp"
#include "scicpp/core/range.hpp"

#include <iostream>

namespace scicpp::polynomial {

TEST_CASE("polyval") {
    SECTION("From std::array") {
        const double v = polyval(5., linspace<5>(0., 10.));
        REQUIRE(almost_equal(v, 7325.));
    }

    SECTION("array, array") {
        const std::array c{4., 5., 8.};
        const std::array x{1., 2., 3.};
        const auto values = polyval(x, c);
        REQUIRE(almost_equal(values, {17., 46., 91.}));
    }

    SECTION("array, vector") {
        const std::vector c{4., 5., 8.};
        const std::array x{1., 2., 3.};
        const auto values = polyval(x, c);
        REQUIRE(almost_equal(values, {17., 46., 91.}));
    }

    SECTION("vector, array") {
        const std::array c{4., 5., 8.};
        const std::vector x{1., 2., 3.};
        const auto values = polyval(x, c);
        REQUIRE(almost_equal(values, {17., 46., 91.}));
    }

    SECTION("vector, vector") {
        const std::vector c{4., 5., 8.};
        const std::vector x{1., 2., 3.};
        const auto values = polyval(x, c);
        REQUIRE(almost_equal(values, {17., 46., 91.}));
    }

    SECTION("From std::vector") {
        const double v = polyval(2.7, linspace(0.0, 3.14, 10));
        REQUIRE(almost_equal(v, 35544.0537997459978214));
    }

    SECTION("Polynomial") {
        const std::array c{4.0, 5.0, 8.0};
        const auto P = Polynomial(c);
        REQUIRE(almost_equal<2>(P(2.7), 75.82));
    }
}

TEST_CASE("Constructors") {
    SECTION("From degree") {
        const auto P = Polynomial<double>(10);
        REQUIRE(P.degree() == 10);
    }

    SECTION("From std::vector") {
        const auto coef = random::rand<double>(10);
        const auto P = Polynomial(coef);
        REQUIRE(P.degree() == 9);
        const auto P2 = Polynomial(random::rand<double>(5));
        REQUIRE(P2.degree() == 4);
    }

    SECTION("From std::array") {
        const auto coef = random::rand<double, 5>();
        const auto P = Polynomial(coef);
        REQUIRE(P.degree() == 4);
    }

    SECTION("From std::iterators") {
        const auto coef = random::rand<double, 5>();
        const auto P = Polynomial<double>(coef.begin(), coef.end());
        REQUIRE(P.degree() == 4);
    }

    SECTION("From initializer list") {
        const Polynomial P{4., 5., 8.};
        REQUIRE(P.degree() == 2);
    }
}

TEST_CASE("Multiply by X") {
    Polynomial P{4., 5., 8.};
    P.mulx();
    REQUIRE(P.is_approx(Polynomial{0., 4., 5., 8.}));
}

TEST_CASE("Operator +=") {
    SECTION("deg(rhs) < deg(this)") {
        Polynomial P{4., 5., 8.};
        P += Polynomial{3., 2.};
        REQUIRE(P.is_approx(Polynomial{7., 7., 8.}));
    }

    SECTION("deg(rhs) > deg(this)") {
        Polynomial P{4., 5., 8.};
        P += Polynomial{3., 2., 9., 12., 42.};
        REQUIRE(P.is_approx(Polynomial{7., 7., 17., 12., 42.}));
    }
}

TEST_CASE("Operator -=") {
    SECTION("deg(rhs) < deg(this)") {
        Polynomial P{4., 5., 8.};
        P -= Polynomial{3., 2.};
        REQUIRE(P.is_approx(Polynomial{1., 3., 8.}));
    }

    SECTION("deg(rhs) > deg(this)") {
        Polynomial P{4., 5., 8.};
        P -= Polynomial{3., 2., 9., 12., 42.};
        REQUIRE(P.is_approx(Polynomial{1., 3., -1., -12., -42.}));
    }
}

TEST_CASE("Operator *=") {
    Polynomial P{1., 2., 3.};
    P *= Polynomial{9., 5., 1.};
    REQUIRE(P.is_approx(Polynomial{9., 23., 38., 17., 3.}));
}

TEST_CASE("Operator +") {
    SECTION("deg(rhs) < deg(this)") {
        const auto P = Polynomial{4., 5., 8.} + Polynomial{3., 2.};
        REQUIRE(P.is_approx(Polynomial{7., 7., 8.}));
    }

    SECTION("deg(rhs) > deg(this)") {
        const auto P =
            Polynomial{4., 5., 8.} + Polynomial{3., 2., 9., 12., 42.};
        REQUIRE(P.is_approx(Polynomial{7., 7., 17., 12., 42.}));
    }
}

TEST_CASE("Operator -") {
    SECTION("deg(rhs) < deg(this)") {
        const auto P = Polynomial{4., 5., 8.} - Polynomial{3., 2.};
        REQUIRE(P.is_approx(Polynomial{1., 3., 8.}));
    }

    SECTION("deg(rhs) > deg(this)") {
        const auto P =
            Polynomial{4., 5., 8.} - Polynomial{3., 2., 9., 12., 42.};
        REQUIRE(P.is_approx(Polynomial{1., 3., -1., -12., -42.}));
    }
}

TEST_CASE("Operator *") {
    const auto P = Polynomial{1., 2., 3.} * Polynomial{9., 5., 1.};
    REQUIRE(P.is_approx(Polynomial{9., 23., 38., 17., 3.}));
}

TEST_CASE("Operator *= scalar") {
    Polynomial P{4., 5., 8.};
    P *= 2.0;
    REQUIRE(P.is_approx(Polynomial{8., 10., 16.}));
}

TEST_CASE("Operator * scalar") {
    SECTION("Scalar * Poly") {
        const auto P = 2.0 * Polynomial{4., 5., 8.};
        REQUIRE(P.is_approx(Polynomial{8., 10., 16.}));
    }

    SECTION("Poly * Scalar") {
        const auto P = Polynomial{4., 5., 8.} * 2.0;
        REQUIRE(P.is_approx(Polynomial{8., 10., 16.}));
    }
}

TEST_CASE("Operator ==") {
    const Polynomial P1{4., 5., 8.};
    const auto P2 = P1;
    REQUIRE(P1 == P2);
}

TEST_CASE("Operator !=") {
    SECTION("Different lengths") {
        const Polynomial P1{4., 5., 8.};
        const Polynomial P2{4., 5., 8., 10., 42.};
        REQUIRE(P1 != P2);
    }

    SECTION("Different values") {
        const Polynomial P1{4., 5., 8.};
        const Polynomial P2{8., 10., 42.};
        REQUIRE(P1 != P2);
    }
}

TEST_CASE("is_approx") {
    SECTION("Identical") {
        const Polynomial P1{4., 5., 8.};
        const auto P2 = P1;
        REQUIRE(P1.is_approx(P2));
    }

    SECTION("Different lengths") {
        const Polynomial P1{4., 5., 8.};
        const Polynomial P2{4., 5., 8., 10., 42.};
        REQUIRE(!P1.is_approx(P2));
    }

    SECTION("Different values") {
        const Polynomial P1{4., 5., 8.};
        const Polynomial P2{8., 10., 42.};
        REQUIRE(!P1.is_approx(P2));
    }
}

TEST_CASE("trim") {
    SECTION("tol = 0") {
        Polynomial P{0., 0., 3., 0., 5., 0., 0.};
        P.trim();
        REQUIRE(P.is_approx(Polynomial{0., 0., 3., 0., 5.}));
    }

    SECTION("tol != 0") {
        Polynomial P{0., 0., 1E-3, 0., 1E-5, 0., 0.};
        P.trim(1E-3);
        REQUIRE(P.is_approx(Polynomial{0.}));
    }
}

TEST_CASE("polyfromroots") {
    SECTION("std::array of roots") {
        const auto P = polyfromroots(std::array{8.0, 42., 78.});
        REQUIRE(P.is_approx(Polynomial{-26208., 4236., -128., 1.}));
    }

    SECTION("std::vector of roots") {
        const auto P = polyfromroots(std::vector{12.0, 54., 85., 44.});
        REQUIRE(P.is_approx(Polynomial{2423520., -330432., 12902., -195., 1.}));
    }
}

TEST_CASE("polyadd") {
    SECTION("std::array") {
        SECTION("deg(P1) < deg(P2)") {
            const auto P = polyadd(std::array{4., 5., 8.}, std::array{3., 2.});
            REQUIRE(almost_equal(P, {7., 7., 8.}));
        }

        SECTION("deg(P1) > deg(P2)") {
            const auto P = polyadd(std::array{4., 5., 8.},
                                   std::array{3., 2., 9., 12., 42.});
            REQUIRE(almost_equal(P, {7., 7., 17., 12., 42.}));
        }
    }

    SECTION("std::vector") {
        SECTION("deg(P1) < deg(P2)") {
            const auto P =
                polyadd(std::vector{4., 5., 8.}, std::vector{3., 2.});
            REQUIRE(almost_equal(P, {7., 7., 8.}));
        }

        SECTION("deg(P1) > deg(P2)") {
            const auto P = polyadd(std::vector{4., 5., 8.},
                                   std::vector{3., 2., 9., 12., 42.});
            REQUIRE(almost_equal(P, {7., 7., 17., 12., 42.}));
        }
    }
}

TEST_CASE("polysub") {
    SECTION("std::array") {
        SECTION("deg(P1) < deg(P2)") {
            const auto P = polysub(std::array{4., 5., 8.}, std::array{3., 2.});
            REQUIRE(almost_equal(P, {1., 3., 8.}));
        }

        SECTION("deg(P1) > deg(P2)") {
            const auto P = polysub(std::array{4., 5., 8.},
                                   std::array{3., 2., 9., 12., 42.});
            REQUIRE(almost_equal(P, {1., 3., -1., -12., -42.}));
        }
    }

    SECTION("std::vector") {
        SECTION("deg(P1) < deg(P2)") {
            const auto P =
                polysub(std::vector{4., 5., 8.}, std::vector{3., 2.});
            REQUIRE(almost_equal(P, {1., 3., 8.}));
        }

        SECTION("deg(P1) > deg(P2)") {
            const auto P = polysub(std::vector{4., 5., 8.},
                                   std::vector{3., 2., 9., 12., 42.});
            REQUIRE(almost_equal(P, {1., 3., -1., -12., -42.}));
        }
    }
}

namespace {

template <class Coefs>
void test_polymul(const Coefs &P1, const Coefs &P2) {
    const auto res = polymul(P1, P2);
    REQUIRE(almost_equal(res, {9., 23., 38., 17., 3.}));
}

TEST_CASE("polymul") {
    test_polymul(std::array{1., 2., 3.}, std::array{9., 5., 1.});
    test_polymul(std::vector{1., 2., 3.}, std::vector{9., 5., 1.});
}

} // namespace

TEST_CASE("polydiv") {
    SECTION("std::array") {
        const std::array u{1., 2., 3.};
        const std::array v{3., 2., 1.};
        const auto [q, r] = polydiv(u, v);
        REQUIRE(almost_equal(q, {3.}));
        REQUIRE(almost_equal(r, {-8., -4.}));

        const std::array u1{8., 10., -5., 3.};
        const std::array v1{-3., 2., 1.};

        const auto [q1, r1] = polydiv(u1, v1);
        REQUIRE(almost_equal(q1, {-11, 3.}));
        REQUIRE(almost_equal(r1, {-25., 41.}));

        const auto [q2, r2] = polydiv(v1, u1);
        REQUIRE(almost_equal(q2, {0.}));
        REQUIRE(almost_equal(r2, {-3., 2., 1.}));

        const auto [q3, r3] = polydiv(u1, std::array{0.5});
        REQUIRE(almost_equal(q3, {16., 20., -10., 6.}));
        REQUIRE(almost_equal(r3, {0.}));
    }

    SECTION("std::vector") {
        const std::vector u{1., 2., 3.};
        const std::vector v{3., 2., 1.};
        const auto [q, r] = polydiv(u, v);
        REQUIRE(almost_equal(q, {3.}));
        REQUIRE(almost_equal(r, {-8., -4.}));

        const std::vector u1{8., 10., -5., 3.};
        const std::vector v1{-3., 2., 1.};

        const auto [q1, r1] = polydiv(u1, v1);
        REQUIRE(almost_equal(q1, {-11, 3.}));
        REQUIRE(almost_equal(r1, {-25., 41.}));

        const auto [q2, r2] = polydiv(v1, u1);
        REQUIRE(almost_equal(q2, {0.}));
        REQUIRE(almost_equal(r2, {-3., 2., 1.}));

        const auto [q3, r3] = polydiv(u1, std::vector{0.5});
        REQUIRE(almost_equal(q3, {16., 20., -10., 6.}));
        REQUIRE(almost_equal(r3, {0.}));
    }
}

TEST_CASE("polymulx") {
    const auto Pa = polymulx(std::array{4., 5., 8.});
    REQUIRE(almost_equal(Pa, {0., 4., 5., 8.}));
    const auto Pv = polymulx(std::vector{4., 5., 8.});
    REQUIRE(almost_equal(Pv, {0., 4., 5., 8.}));
    REQUIRE(almost_equal(polymulx(std::vector{4., 5., 8.}), {0., 4., 5., 8.}));
}

TEST_CASE("polypow") {
    SECTION("std::array") {
        const auto P = polypow<3>(std::array{4., 5., 8.});
        REQUIRE(
            almost_equal<2>(P, {64., 240., 684., 1085., 1368., 960., 512.}));
    }

    SECTION("std::vector DIRECT") {
        const auto P = polypow(std::vector{4., 5., 8.}, 3);
        REQUIRE(
            almost_equal<2>(P, {64., 240., 684., 1085., 1368., 960., 512.}));
    }

    SECTION("std::vector FFT") {
        const auto P = polypow<signal::FFT>(std::vector{4., 5., 8.}, 3);
        REQUIRE(
            almost_equal<4>(P, {64., 240., 684., 1085., 1368., 960., 512.}));
    }
}

TEST_CASE("polyder") {
    SECTION("std::array") {
        const std::array a{4., 5., 8., 3.14, 42.};
        REQUIRE(almost_equal(polyder(a), {16., 15., 16., 3.14}));
        REQUIRE(almost_equal(polyder<3>(a), {96., 30.}));
        REQUIRE(almost_equal(polyder<0>(a), a));
        REQUIRE(almost_equal(polyder(std::array<double, 0>{}), {}));
        REQUIRE(almost_equal(polyder<10>(a), {}));
    }

    SECTION("std::vector") {
        const std::vector a{4., 5., 8., 3.14, 42.};
        // print(polyder(a));
        REQUIRE(almost_equal(polyder(a), {16., 15., 16., 3.14}));
        REQUIRE(almost_equal(polyder(a, 3), {96., 30.}));
        REQUIRE(almost_equal(polyder(a, 0), a));
        REQUIRE(almost_equal(polyder(std::vector<double>(0)), {}));
        REQUIRE(almost_equal(polyder(a, 10), {}));
    }
}

TEST_CASE("polyint") {
    SECTION("std::array") {
        const std::array a{4., 5., 8., 3.14, 42.};
        // print(polyint<2>(a));
        REQUIRE(almost_equal(polyint<0>(a), a));
        REQUIRE(almost_equal(polyint(a), {0., 4., 2.5, 8. / 3., 0.785, 8.4}));
        REQUIRE(almost_equal<2>(polyint<2>(a),
                                {0., 0., 2., 2.5 / 3., 2. / 3., 0.157, 1.4}));
    }

    SECTION("std::vector") {
        const std::vector a{4., 5., 8., 3.14, 42.};
        // print(polyint(a));
        REQUIRE(almost_equal(polyint(a, 0), a));
        REQUIRE(almost_equal(polyint(a), {0., 4., 2.5, 8. / 3., 0.785, 8.4}));
        REQUIRE(almost_equal<2>(polyint(a, 2),
                                {0., 0., 2., 2.5 / 3., 2. / 3., 0.157, 1.4}));
    }
}

TEST_CASE("polytrim") {
    SECTION("tol = 0") {
        std::vector P{0., 0., 3., 0., 5., 0., 0.};
        polytrim(P);
        REQUIRE(almost_equal(P, {0., 0., 3., 0., 5.}));
    }

    SECTION("tol != 0") {
        std::vector P{0., 0., 1E-3, 0., 1E-5, 0., 0.};
        polytrim(P, 1E-3);
        REQUIRE(almost_equal(P, {0.}));
    }
}

namespace {

template <class Coefs>
void test_polycompanion(const Coefs &P) {
    const auto c = polycompanion(P);
    REQUIRE(c.rows() == c.cols());
    REQUIRE(c.rows() == int(P.size()) - 1);
    Eigen::Matrix<double, 4, 4> m;
    m << 0., 0., 0., -0.2, //
        1., 0., 0., -0.4,  //
        0., 1., 0., -0.6,  //
        0., 0., 1., -0.8;  //
    // std::cout << m << std::endl;
    // std::cout << c << std::endl;
    REQUIRE(m.isApprox(c));
}

TEST_CASE("polycompanion") {
    test_polycompanion(std::array{1., 2., 3., 4., 5.});
    test_polycompanion(std::vector{1., 2., 3., 4., 5.});
}

} // namespace

namespace {

template <class Coefs>
void test_polyroots(const Coefs &P) {
    const auto r0 = 0.1378322749029896 + 0.6781543891053361i;
    const auto r1 = 0.1378322749029896 - 0.6781543891053361i;
    const auto r2 = -0.5378322749029902 + 0.3582846863451272i;
    const auto r3 = -0.5378322749029902 - 0.3582846863451272i;

    const auto roots = polyroots(P);
    const decltype(roots) expected_roots{r0, r1, r2, r3};
    REQUIRE(almost_equal<15>(roots, expected_roots));
}

TEST_CASE("polyroots") {
    test_polyroots(std::array{1., 2., 3., 4., 5.});
    test_polyroots(std::vector{1., 2., 3., 4., 5.});
}

} // namespace

TEST_CASE("polyvander") {
    SECTION("std::array") {
        const std::array x{1., 2., 3., 4., 5.};
        const auto V = polyvander<4>(x);
        REQUIRE(V.rows() == x.size());
        REQUIRE(V.cols() == 5);
        // std::cout << V << std::endl;
        Eigen::Matrix<double, x.size(), 5> m;
        m << 1., 1., 1., 1., 1.,     //
            1., 2., 4., 8., 16.,     //
            1., 3., 9., 27., 81.,    //
            1., 4., 16., 64., 256.,  //
            1., 5., 25., 125., 625.; //
        REQUIRE(m.isApprox(V));
    }

    SECTION("std::vector") {
        const std::vector x{1., 2., 3., 4., 5.};
        const auto V = polyvander(x, 3);
        REQUIRE(V.rows() == x.size());
        REQUIRE(V.cols() == 4);
        // std::cout << V << std::endl;
        Eigen::Matrix<double, 5, 4> m;
        m << 1., 1., 1., 1.,   //
            1., 2., 4., 8.,    //
            1., 3., 9., 27.,   //
            1., 4., 16., 64.,  //
            1., 5., 25., 125.; //
        REQUIRE(m.isApprox(V));
    }
}

TEST_CASE("polyfit") {
    SECTION("std::array") {
        const std::array x{1., 2., 3., 4., 5.};
        const std::array y{6., 7., 8., 9., 10.};
        const auto P = polyfit<1>(x, y);
        REQUIRE(almost_equal<4>(P, {5., 1.}));
    }

    SECTION("std::vector") {
        const std::vector x{1., 1.1, 3., 4.3, 8.};
        const std::vector y{6., 3., 5., 9., 42.};
        const auto P = polyfit(x, y, 4);
        // printf("%.16f\n%.16f\n%.16f\n%.16f\n%.16f\n",
        //        P[0], P[1], P[2], P[3], P[4]);
        REQUIRE(almost_equal<800>(P,
                                  {77.27908880037437,
                                   -119.03652993676596,
                                   57.779774947055685,
                                   -10.678177132411323,
                                   0.6558433217472622}));
    }
}

} // namespace scicpp::polynomial