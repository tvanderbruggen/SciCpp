#include "constants.hpp"

#include "scicpp/core/equal.hpp"
#include "scicpp/core/units.hpp"

namespace scicpp {

TEST_CASE("Physical constants") {
    using namespace units::literals;
    REQUIRE(almost_equal(cst::c<long double>, 299792.458_km_per_s));
    REQUIRE(
        almost_equal(static_cast<long double>(1E34) * cst::hbar<long double>,
                     1.05457181764615634794_J * 1._s));
}

} // namespace scicpp