// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include "arithmetic.hpp"

#include "scicpp/core/constants.hpp"
#include "scicpp/core/equal.hpp"

#include <cmath>

namespace scicpp::arithmetic {

TEST_CASE("compile-time root") {
    static_assert(root<2>(1) == 1);
    static_assert(root<2>(9) == 3);
    static_assert(root<3>(10648) == 22);
    REQUIRE(std::isnan(root<2>(-2.)));
    REQUIRE(almost_equal(root<2>(22.), 4.690415759823429554566));
    REQUIRE(almost_equal(root<5>(pi<double>), 1.257274115669185059385));
}

TEST_CASE("prime_factors") {
    constexpr auto factors = prime_factors<84628766448>().values();
    static_assert(factors[0].first == 2 && factors[0].second == 4);
    static_assert(factors[1].first == 3 && factors[1].second == 2);
    static_assert(factors[2].first == 31 && factors[2].second == 1);
    static_assert(factors[3].first == 59 && factors[3].second == 1);
    static_assert(factors[4].first == 321323 && factors[4].second == 1);
    // print(factors);
}

TEST_CASE("root_ratio") {
    SECTION("is_exact_root") {
        static_assert(detail::is_exact_root(prime_factors<4>().values(), 2));
        static_assert(!detail::is_exact_root(prime_factors<10>().values(), 2));
        static_assert(
            detail::is_exact_root(prime_factors<23435281>().values(), 2));
    }

    SECTION("Root") {
        using R = root_ratio<std::ratio<1, 4>>;

        static_assert(std::is_same_v<root_ratio_root<R, 2>,
                                     root_ratio<std::ratio<1, 2>>>);
        static_assert(std::is_same_v<root_ratio_root<R, 3>,
                                     root_ratio<std::ratio<1, 4>, 3>>);
    }

    SECTION("Multiply / Exponent = 1") {
        using R1 = root_ratio<std::ratio<1, 2>>;
        using R2 = root_ratio<std::ratio<3>>;
        using RMult = root_ratio_multiply<R1, R2>;

        static_assert(RMult::num == 3 && RMult::den == 2);
    }

    SECTION("Multiply / Exponent != 1") {
        using R1 = root_ratio<std::ratio<1, 2>, 2>;
        using R2 = root_ratio<std::ratio<3>, 3>;
        using R3 = root_ratio<std::ratio<1, 3>, 2>;
        using R4 = root_ratio<std::ratio<1, 2>, 3>;

        static_assert(std::is_same_v<root_ratio_multiply<R1, R2>,
                                     root_ratio<std::ratio<9, 8>, 6>>);
        static_assert(std::is_same_v<root_ratio_multiply<R1, R3>,
                                     root_ratio<std::ratio<1, 6>, 2>>);
        static_assert(std::is_same_v<root_ratio_multiply<R1, R4>,
                                     root_ratio<std::ratio<1, 32>, 6>>);
        static_assert(std::is_same_v<root_ratio_multiply<R1, R1>,
                                     root_ratio<std::ratio<1, 2>>>);
        static_assert(
            std::is_same_v<root_ratio_multiply<R1, root_ratio_multiply<R1, R1>>,
                           root_ratio<std::ratio<1, 8>, 2>>);
    }

    SECTION("Divide") {
        using R1 = root_ratio<std::ratio<1, 2>, 2>;
        using R2 = root_ratio<std::ratio<3>, 3>;

        static_assert(std::is_same_v<root_ratio_divide<R1, R1>,
                                     root_ratio<std::ratio<1>>>);
        static_assert(std::is_same_v<root_ratio_multiply<R1, R2>,
                                     root_ratio<std::ratio<9, 8>, 6>>);
    }
}

} // namespace scicpp::arithmetic