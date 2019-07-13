#include "constants.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/units.hpp"

namespace scicpp {

TEST_CASE("Physical constants") {
    using namespace units::literals;
    REQUIRE(almost_equal(phys_cst::c, 299792.458_km_per_s));
    REQUIRE(
        almost_equal<2>(1E34 * phys_cst::hbar, 1.05457181764615634794_J * 1_s));
    REQUIRE(almost_equal<2>(1E6 * phys_cst::mu0,
                            1.256637062123837634_N / 1_A / 1_A));
    REQUIRE(
        almost_equal<2>(1E12 * phys_cst::epsilon0, 8.854187812773345_F / 1_m));
    REQUIRE(almost_equal<2>(phys_cst::K_J, 483597.848416983625E9_Hz / 1_V));
    REQUIRE(almost_equal<2>(phys_cst::R_K, 25812.807459304513031_Ohm));
    REQUIRE(almost_equal<2>(1E15 * phys_cst::Phi0, 2.067833848461929414_Wb));
    REQUIRE(
        almost_equal<2>(1E24 * phys_cst::muB, 9.274010078362163512_J / 1_T));
    REQUIRE(
        almost_equal<2>(1E27 * phys_cst::muN, 5.050783746096374927_J / 1_T));
    // printf("%.18Lf\n", phys_cst_l::Rinf.value());
    // NB: Discrepency with CODATA numerical value (10973731.568160)
    REQUIRE(almost_equal<2>(phys_cst::Rinf, 10973731.568138644099235535 / 1_m));
    // printf("%.18f\n", 1E11 * phys_cst::a0.value());
    REQUIRE(almost_equal<2>(1E11 * phys_cst::a0, 5.291772109044924122_m));
}

} // namespace scicpp