// SPDX-License-Identifier: MIT
// Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "range.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/numeric.hpp"
#include "scicpp/core/print.hpp"
#include "scicpp/core/units/units.hpp"

#include <complex>

namespace scicpp {

TEST_CASE("empty") {
    REQUIRE(empty<double>().empty());
}

TEST_CASE("full") {
    SECTION("std::array") {
        REQUIRE(full<0>(2.).empty());
        REQUIRE(almost_equal(full<1>(2.), {2.}));
        REQUIRE(almost_equal(full<5>(2.), {2., 2., 2., 2., 2.}));

        using namespace units::literals;
        REQUIRE(almost_equal(full<5>(2._m), {2._m, 2._m, 2._m, 2._m, 2._m}));
        REQUIRE(
            almost_equal(full<2>(std::complex(2._m, 1._m)),
                         {std::complex(2._m, 1._m), std::complex(2._m, 1._m)}));
    }

    SECTION("std::vector") {
        REQUIRE(full(0, 2.).empty());
        REQUIRE(almost_equal(full(1, 2.), {2.}));
        REQUIRE(almost_equal(full(5, 2.), {2., 2., 2., 2., 2.}));

        using namespace units::literals;
        REQUIRE(almost_equal(full(5, 2._m), {2._m, 2._m, 2._m, 2._m, 2._m}));
        REQUIRE(
            almost_equal(full(2, std::complex(2._m, 1._m)),
                         {std::complex(2._m, 1._m), std::complex(2._m, 1._m)}));
    }
}

TEST_CASE("zeros") {
    SECTION("std::array") {
        REQUIRE(zeros<0, double>().empty());
        REQUIRE(almost_equal(zeros<1, double>(), {0.}));
        REQUIRE(almost_equal(zeros<5, double>(), {0., 0., 0., 0., 0.}));
        REQUIRE(almost_equal(zeros<1, std::complex<double>>(), {0. + 0.i}));

        using namespace units::literals;
        REQUIRE(almost_equal(zeros<1, units::pascal<double>>(), {0_Pa}));
        REQUIRE(almost_equal(zeros<1, std::complex<units::pascal<double>>>(),
                             {std::complex(0_Pa, 0_Pa)}));
        REQUIRE(almost_equal(zeros<3, std::complex<units::kilogram<double>>>(),
                             {std::complex(0_kg, 0_kg),
                              std::complex(0_kg, 0_kg),
                              std::complex(0_kg, 0_kg)}));
    }

    SECTION("std::vector") {
        REQUIRE(zeros<double>(0).empty());
        REQUIRE(almost_equal(zeros<double>(1), {0.}));
        REQUIRE(almost_equal(zeros<double>(5), {0., 0., 0., 0., 0.}));

        using namespace units::literals;
        REQUIRE(almost_equal(zeros<units::pascal<double>>(1), {0_Pa}));
        REQUIRE(almost_equal(zeros<std::complex<units::pascal<double>>>(1),
                             {std::complex(0_Pa, 0_Pa)}));
        REQUIRE(almost_equal(zeros<std::complex<units::kilogram<double>>>(3),
                             {std::complex(0_kg, 0_kg),
                              std::complex(0_kg, 0_kg),
                              std::complex(0_kg, 0_kg)}));
    }
}

TEST_CASE("ones") {
    SECTION("std::array") {
        REQUIRE(ones<0, double>().empty());
        REQUIRE(almost_equal(ones<1, double>(), {1.}));
        REQUIRE(almost_equal(ones<5, double>(), {1., 1., 1., 1., 1.}));
        REQUIRE(almost_equal(ones<1, std::complex<double>>(), {1. + 0.i}));

        using namespace units::literals;
        REQUIRE(almost_equal(ones<1, units::pascal<double>>(), {1_Pa}));
        REQUIRE(almost_equal(ones<1, std::complex<units::pascal<double>>>(),
                             {std::complex(1_Pa, 0_Pa)}));
        REQUIRE(almost_equal(ones<3, std::complex<units::kilogram<double>>>(),
                             {std::complex(1_kg, 0_kg),
                              std::complex(1_kg, 0_kg),
                              std::complex(1_kg, 0_kg)}));
    }

    SECTION("std::vector") {
        REQUIRE(ones<double>(0).empty());
        REQUIRE(almost_equal(ones<double>(1), {1.}));
        REQUIRE(almost_equal(ones<double>(5), {1., 1., 1., 1., 1.}));

        using namespace units::literals;
        REQUIRE(almost_equal(ones<units::pascal<double>>(1), {1_Pa}));
        REQUIRE(almost_equal(ones<std::complex<units::pascal<double>>>(1),
                             {std::complex(1_Pa, 0_Pa)}));
        REQUIRE(almost_equal(ones<std::complex<units::kilogram<double>>>(3),
                             {std::complex(1_kg, 0_kg),
                              std::complex(1_kg, 0_kg),
                              std::complex(1_kg, 0_kg)}));
    }
}

TEST_CASE("linspace") {
    SECTION("std::array") {
        REQUIRE(linspace<0>(2., 3.).empty());
        REQUIRE(almost_equal(linspace<1>(2., 3.), {2.}));
        REQUIRE(almost_equal(linspace<5>(2., 3.), {2., 2.25, 2.5, 2.75, 3.}));
        REQUIRE(almost_equal(linspace<5>(3., 2.), {3., 2.75, 2.5, 2.25, 2.}));
    }

    SECTION("std::array with quantity") {
        using namespace units::literals;
        REQUIRE(linspace<0>(2._m, 3._m).empty());
        REQUIRE(almost_equal(linspace<1>(2._m, 3._m), {2._m}));
        REQUIRE(almost_equal(linspace<5>(2._m, 3._m),
                             {2._m, 2.25_m, 2.5_m, 2.75_m, 3._m}));
        REQUIRE(almost_equal(linspace<5>(3._kg, 2._kg),
                             {3._kg, 2.75_kg, 2.5_kg, 2.25_kg, 2._kg}));
    }

    SECTION("std::vector") {
        REQUIRE(linspace(2., 3., 0).empty());
        REQUIRE(almost_equal(linspace(2., 3., 1), {2.}));
        REQUIRE(almost_equal(linspace(2., 3., 5), {2., 2.25, 2.5, 2.75, 3.}));
        REQUIRE(almost_equal(linspace(3., 2., 5), {3., 2.75, 2.5, 2.25, 2.}));
    }

    SECTION("std::vector with quantity") {
        using namespace units::literals;
        REQUIRE(linspace(2._m, 3._m, 0).empty());
        REQUIRE(almost_equal(linspace(2._m, 3._m, 1), {2._m}));
        REQUIRE(almost_equal(linspace(2._m, 3._m, 5),
                             {2._m, 2.25_m, 2.5_m, 2.75_m, 3._m}));
        REQUIRE(almost_equal(linspace(3._kg, 2._kg, 5),
                             {3._kg, 2.75_kg, 2.5_kg, 2.25_kg, 2._kg}));
    }
}

TEST_CASE("logspace std::array") {
    SECTION("std::array") {
        REQUIRE(logspace<0>(2., 3.).empty());
        REQUIRE(almost_equal(logspace<1>(2., 3.), {100.}));
        REQUIRE(almost_equal(
            logspace<4>(2., 3.),
            {100., 215.44346900318845, 464.15888336127773, 1000.}));
    }

    SECTION("std::array with quantities") {
        using namespace units::literals;
        REQUIRE(logspace<0, units::meter<double>>(2., 3.).empty());
        REQUIRE(
            almost_equal(logspace<1, units::meter<double>>(2., 3.), {100_m}));
        print(logspace<4, units::mass<double>>(2., 3.));
        REQUIRE(almost_equal(
            logspace<4, units::mass<double>>(2., 3.),
            {100_kg, 215.44346900318845_kg, 464.15888336127773_kg, 1000_kg}));
    }

    SECTION("std::vector") {
        REQUIRE(logspace(2., 3., 0).empty());
        REQUIRE(almost_equal(logspace(2., 3., 1), {100.}));
        REQUIRE(almost_equal(
            logspace(2., 3., 4),
            {100., 215.44346900318845, 464.15888336127773, 1000.}));
    }

    SECTION("std::vector with quantities") {
        using namespace units::literals;
        REQUIRE(logspace<units::meter<double>>(2., 3., 0).empty());
        REQUIRE(
            almost_equal(logspace<units::meter<double>>(2., 3., 1), {100_m}));
        REQUIRE(almost_equal(
            logspace<units::mass<double>>(2., 3., 4),
            {100_kg, 215.44346900318845_kg, 464.15888336127773_kg, 1000_kg}));
    }
}

TEST_CASE("arange std::vector") {
    REQUIRE(arange(2., 3., 10.).empty());
    REQUIRE(almost_equal(arange(2., 3., 0.75), {2.}));
    REQUIRE(almost_equal(arange(0., 3.), {0., 1., 2.}));
    REQUIRE(arange(1., 0., 0.5).empty());
    REQUIRE(arange(0., 3., -1.).empty());
    REQUIRE(almost_equal(arange(3., 0., -1.), {3., 2., 1.}));
}

TEST_CASE("arange std::vector with quantity") {
    using namespace units::literals;
    REQUIRE(arange(2_m, 3_m, 10_m).empty());
    REQUIRE(almost_equal(arange(2_m, 3_m, 0.75_m), {2_m}));
    REQUIRE(almost_equal(arange(0_m, 3_m), {0_m, 1_m, 2_m}));
    REQUIRE(arange(1_kg, 0_kg, 0.5_kg).empty());
    REQUIRE(arange(0_kg, 3_kg, -1_kg).empty());
    REQUIRE(almost_equal(arange(3_kg, 0_kg, -1_kg), {3_kg, 2_kg, 1_kg}));
}

} // namespace scicpp
