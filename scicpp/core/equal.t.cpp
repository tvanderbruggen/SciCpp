#include "equal.hpp"

#include "scicpp/core/constants.hpp"
#include "scicpp/core/units/units.hpp"

namespace scicpp {

TEST_CASE("almost_equal real") {
    SECTION("NaNs") {
        constexpr auto nan = std::numeric_limits<double>::quiet_NaN();
        REQUIRE(almost_equal(nan, nan));
    }

    SECTION("Inf") {
        constexpr auto inf = std::numeric_limits<double>::infinity();
        REQUIRE(almost_equal(inf, inf));
        REQUIRE(!almost_equal(inf, -inf));
    }

    SECTION("Zero") {
        REQUIRE(almost_equal(0., 0.));
        REQUIRE(almost_equal(-0., -0.));
        REQUIRE(almost_equal(0., -0.));
    }

    SECTION("near zero") {
        REQUIRE(almost_equal(0., 0.00000000000000011102));
        REQUIRE(almost_equal(0.00000000000000011102, 0.));
        REQUIRE(almost_equal(0.00000000000000011102, 0.00000000000000011102));
    }

    SECTION("std::array") {
        const std::array f1{1., 2., 3.141};
        const auto f2 = f1;
        REQUIRE(almost_equal(f1, f2));
    }

    SECTION("std::vector") {
        const std::vector f1{1., 2., 3.141};
        const auto f2 = f1;
        REQUIRE(almost_equal(f1, f2));

        const std::vector f3{1., 2.};
        REQUIRE(!almost_equal(f1, f3));

        const std::vector f4{1., 2.1, 3.141};
        REQUIRE(!almost_equal(f1, f4));
    }
}

TEST_CASE("almost_equal complex") {
    SECTION("scalar") {
        constexpr auto eps = std::numeric_limits<double>::epsilon();
        const auto z1 = 3.141 + 42.i;
        const auto z2 = 3.141 + 2. * eps + 42.i;
        REQUIRE(almost_equal(z1, z1));
        REQUIRE(!almost_equal(z1, z2));
        REQUIRE(almost_equal<2>(z1, z2));
    }

    SECTION("std::array") {
        const std::array f1{1. + 2.i, 3.141 + 42.i};
        const auto f2 = f1;
        REQUIRE(almost_equal(f1, f2));
    }

    SECTION("std::vector") {
        const std::vector f1{1. + 2.i, 3.141 + 42.i};
        const auto f2 = f1;
        REQUIRE(almost_equal(f1, f2));
    }
}

TEST_CASE("almost_equal physical quantity") {
    using namespace units::literals;

    SECTION("std::array") {
        const std::array f1{1._kg, 2._kg, 3.141_kg};
        const auto f2 = f1;
        REQUIRE(almost_equal(f1, f2));
    }

    SECTION("std::vector") {
        const std::vector f1{1._kg, 2._kg, 3.141_kg};
        const auto f2 = f1;
        REQUIRE(almost_equal(f1, f2));
    }
}

TEST_CASE("operator==") {
    SECTION("std::array") {
        const std::array f1{1, 2, 3};
        const auto f2 = f1;
        REQUIRE(array_equal(f1, f2));
        REQUIRE(!array_equal(f1, {1, 2, 42}));
        REQUIRE(!array_equal(f1, {1, 2}));
    }

    SECTION("std::vector") {
        const std::vector f1{1, 2, 3};
        const auto f2 = f1;
        REQUIRE(array_equal(f1, f2));
        REQUIRE(!array_equal(f1, {1, 2, 42}));
        REQUIRE(!array_equal(f1, {1, 2}));
    }
}

TEST_CASE("strings_equal") {
    static_assert(strings_equal("str1", "str1"));
    static_assert(!strings_equal("str1", "str2"));
}

} // namespace scicpp