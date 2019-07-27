.. Scicpp documentation master file, created by
   sphinx-quickstart on Sat Dec  8 16:32:58 2018.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

SciCpp's documentation
==================================

SciCpp is a data and signal processing C++ library.

SciCpp objective is to follow the `SciPy <https://scipy.org/>`_ API while using standard C++
containers (``std::array`` and ``std::vector``) whenever possible.

SciCpp is a lightweight header only library.
Its only dependence is the `Eigen <http://eigen.tuxfamily.org/index.php?title=Main_Page>`_
linear algebra library, which is also header only.

SciCpp uses the C++-17 standard.
It is tested with the GCC and Clang compilers under Linux.

SciCpp is released under the MIT licence and `sources are available on Github <https://github.com/tvanderbruggen/SciCpp>`_.

Modules:
--------

.. toctree::
   :maxdepth: 2

   core.rst
   linalg.rst
   polynomial.rst
   signal.rst

Introduction
--------

SciPy is an amazing library that facilitates developments.
Often SciPy is used for prototyping but algorithms must be re-implemented
in a system language for production.
This is especially the case for embedded systems, where the CPU and/or
memory resources may be too limited for the Python runtime.

SciCpp ambitions to facilitate the transition from
SciPy to C++ implementation. Therefore, SciCpp goal is
to mimic the SciPy API in terms of functions naming and behavior,
to avoid surprises in the transition process.

Example
--------

This example load and exploits some real world data.
Data are `wave parameters (height, period, ...) measured by a buoy <https://www.qld.gov.au/environment/coasts-waterways/beach/monitoring>`_.

It presents how SciCpp can be used to load data fron a CSV file, to clean up the data, and to extract information.
::
    #include <cstdio>
    #include <scicpp/core.hpp>

    namespace sci = scicpp;
    using namespace sci::operators;
    using namespace sci::units::literals;

    int main() {
        // -----------------------------------------------------------------------------
        // Load data
        // -----------------------------------------------------------------------------

        // The data fields are defined here:
        // https://www.qld.gov.au/environment/coasts-waterways/beach/monitoring/waves-glossary#wave-height
        //
        // From these definitions, we define the data types for each column
        using Hsig = sci::units::meter<>;  // Significant wave height
        using Hmax = sci::units::meter<>;  // Highest single wave height
        using Tz = sci::units::second<>;   // Average of the wave periods
        using Tp = sci::units::second<>;   // Period of most energetic waves
        using SST = sci::units::celsius<>; // Sea surface temperature

        auto [hsig, hmax, tz, tp, sst] =
            sci::TxtLoader<Hsig, Hmax, Tz, Tp, SST>()
                .delimiter(',')
                .skiprows(1)
                .usecols(1, 2, 3, 4, 6)
                .load<sci::io::unpack>(
                    "examples/townsville_2019-01-01t00_00-2019-06-30t23_30.csv");

        // -----------------------------------------------------------------------------
        // Clean up data
        // -----------------------------------------------------------------------------

        // In the dataset invalid values are represented by -99.9,
        // but only positive values are valid.
        // We create a mask to filter out these values:
        const auto valid_mask = (hsig >= 0_m) && (hmax >= 0_m) && (tz >= 0_s) &&
                                (tp >= 0_s) && (sst >= 0_degC);

        // We then mask the data to keep only the valid values:
        sci::mask_array(hsig, valid_mask);
        sci::mask_array(hmax, valid_mask);
        sci::mask_array(tz, valid_mask);
        sci::mask_array(tp, valid_mask);
        sci::mask_array(sst, valid_mask);

        // -----------------------------------------------------------------------------
        // Explore data
        // -----------------------------------------------------------------------------

        // Some simple stats:
        printf("Highest wave is %.2f m\n", sci::stats::amax(hmax).value());
        printf("Longest period is %.2f s\n", sci::stats::amax(tz).value());
        printf("Average sea surface temperature is %.2f deg. C\n",
            sci::stats::mean(sst).value());

        // Analyze the energetic potential of the waves.
        // https://en.wikipedia.org/wiki/Wave_power
        constexpr auto rho = 1000_kg / 1_m3; // Water density
        constexpr auto g = 9.81_m_per_s2;    // Acceleration by gravity

        // Wave power (per meter of wavefront)
        const auto P = (rho * g * g / (64 * sci::pi<double>)) * hsig * hsig * tz;
        printf("Average wave power %.2f W/m\n", sci::stats::mean(P).value());

        // Wave energy density
        const auto E = (rho * g / 16.) * hsig * hsig;
        printf("Average wave energy density %.2f J/m^2\n",
            sci::stats::mean(E).value());
    }