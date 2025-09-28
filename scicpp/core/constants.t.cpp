#include "constants.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/units/units.hpp"

namespace scicpp {

TEST_CASE("Physical constants") {
    using namespace units::literals;
    REQUIRE(almost_equal(phys_cst::c, 299792.458_km_per_s));
    REQUIRE(
        almost_equal<2>(1E34 * phys_cst::hbar, 1.05457181764615634794_J * 1_s));
    // printf("%.24Lf\n", phys_cst_l::mu0.value());
    REQUIRE(almost_equal<2>(1E6 * phys_cst::mu0,
                            1.256637061262814882_N / 1_A / 1_A));
    // printf("%.30Lf\n", phys_cst_l::epsilon0.value());
    REQUIRE(
        almost_equal<2>(1E12 * phys_cst::epsilon0, 8.854187818840058860_F / 1_m));
    // printf("%.30Lf\n", phys_cst_l::K_J.value());
    REQUIRE(almost_equal<2>(phys_cst::K_J, 483597.848416983625E9_Hz / 1_V));
    // printf("%.30Lf\n", phys_cst_l::R_K.value());
    REQUIRE(almost_equal<2>(phys_cst::R_K, 25812.807459304513031_Ohm));
    // printf("%.30Lf\n", phys_cst_l::Phi0.value());
    REQUIRE(almost_equal<2>(1E15 * phys_cst::Phi0, 2.067833848461929414_Wb));
    // printf("%.30Le\n", phys_cst_l::muB.value());
    REQUIRE(
        almost_equal<2>(1E24 * phys_cst::muB, 9.274010065738068515475452810387_J / 1_T));
    // printf("%.30Le\n", phys_cst_l::muN.value());
    REQUIRE(
        almost_equal<2>(1E27 * phys_cst::muN, 5.050783739271897144306250269724_J / 1_T));
    // printf("%.30Le\n", phys_cst_l::Rinf.value());
    REQUIRE(almost_equal<2>(phys_cst::Rinf, 10973731.56803849312655074754730 / 1_m));
    // printf("%.30Le\n", phys_cst_l::a0.value());
    REQUIRE(almost_equal<2>(1E11 * phys_cst::a0, 5.291772105467402030742718729114_m));
}

} // namespace scicpp