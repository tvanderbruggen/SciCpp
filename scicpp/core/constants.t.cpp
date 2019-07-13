#include "constants.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/units.hpp"

namespace scicpp {

TEST_CASE("Physical constants") {
    using namespace units::literals;
    REQUIRE(almost_equal(phys_cst::c, 299792.458_km_per_s));
    REQUIRE(
        almost_equal<2>(1E34 * phys_cst::hbar, 1.05457181764615634794_J * 1_s));
    REQUIRE(
        almost_equal<2>(1E6 * phys_cst::mu0, 1.256637062123838_N / 1_A / 1_A));
    // printf("%.15f\n", 1E12 * phys_cst::epsilon0.value());
    REQUIRE(
        almost_equal<2>(1E12 * phys_cst::epsilon0, 8.854187812773345_F / 1_m));
}

} // namespace scicpp