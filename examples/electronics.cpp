// SPDX-License-Identifier: MIT
// Copyright (c) 2023 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#include <complex>
#include <cstdio>
#include <scicpp/core.hpp>
#include <type_traits>

namespace sci = scicpp;
using namespace sci::operators;
using namespace sci::units::literals;

// ----------------------------------------------------------------------------
// Power
// ----------------------------------------------------------------------------

template <typename T>
constexpr auto power(sci::units::electric_potential<T> V,
                     sci::units::electric_current<T> I) {
    const auto P = I * V;
    static_assert(sci::units::is_power<decltype(P)>);
    return P;
}

// ----------------------------------------------------------------------------
// Impedance
// ----------------------------------------------------------------------------

using impedance = std::complex<sci::units::electrical_resistance<double>>;

template <int tol = 1>
bool is_resistor(const impedance &Z) {
    return sci::almost_equal<tol>(Z.imag(), 0_Ohm);
}

auto resistance(const impedance &Z) {
    return Z.real();
}

auto reactance(const impedance &Z) {
    return Z.imag();
}

template <typename... Impedances>
auto parallel(Impedances... Z) {
    static_assert((std::is_same_v<Impedances, impedance> && ...));
    return 1.0 / ((1.0 / Z) + ...);
}

auto reflection_coefficient(const impedance &Zsource, const impedance &Zload) {
    const auto R = (Zsource - Zload) / (Zsource + Zload);
    static_assert(sci::units::is_dimensionless<decltype(R.real())>);
    static_assert(sci::units::is_dimensionless<decltype(R.imag())>);
    return sci::units::value(R);
}

auto vswr(const impedance &Zsource, const impedance &Zload) {
    const auto R = sci::absolute(reflection_coefficient(Zsource, Zload));
    return (1.0 + R) / (1.0 - R);
}

auto return_loss(const impedance &Zsource, const impedance &Zload) {
    return -20.0 *
           sci::log10(sci::absolute(reflection_coefficient(Zsource, Zload)));
}

auto get_impedance(sci::units::capacitance<double> C,
                   sci::units::frequency<double> f) {
    return -impedance(0_Ohm, 1.0 / (2.0 * sci::pi<double> * C * f));
}

auto get_impedance(sci::units::inductance<double> L,
                   sci::units::frequency<double> f) {
    return impedance(0_Ohm, 2.0 * sci::pi<double> * L * f);
}

// ----------------------------------------------------------------------------
// RLC filter
// ----------------------------------------------------------------------------

struct rlc_filter {
    rlc_filter(sci::units::electrical_resistance<double> R,
               sci::units::inductance<double> L,
               sci::units::capacitance<double> C)
        : m_R(R), m_L(L), m_C(C) {}

    auto impedance(sci::units::frequency<double> f) const {
        const auto Z1 = m_R + get_impedance(m_L, f);
        const auto Z2 = get_impedance(m_C, f);
        return sci::units::value(Z2 / (Z1 + Z2));
    }

    template <typename T>
    auto operator()(T &&x) const {
        if constexpr (sci::meta::is_iterable_v<T>) {
            return sci::map([&](auto f) { return impedance(f); },
                            std::forward<T>(x));
        } else {
            return impedance(x);
        }
    }

    sci::units::electrical_resistance<double> m_R;
    sci::units::inductance<double> m_L;
    sci::units::capacitance<double> m_C;
};

// ----------------------------------------------------------------------------
// PCB trace
// ----------------------------------------------------------------------------

auto copper_resistivity(sci::units::temperature<double> T = 20_degC) {
    // Copper resistivity at 20 C
    constexpr auto copper_resistivity_20C = 1.72E-8_Ohm * 1_m;
    constexpr auto copper_resistivity_temp_change = 0.00393_Ohm * 1_m / 1_K;
    constexpr auto Tref =
        sci::units::quantity_cast<sci::units::kelvin<double>>(20_degC);

    const auto rho =
        copper_resistivity_20C + copper_resistivity_temp_change * (T - Tref);
    static_assert(sci::units::is_electrical_resistivity<decltype(rho)>);
    return rho;
}

template <typename T>
struct PcbTrace {
    PcbTrace(sci::units::length<T> length,
             sci::units::length<T> width,
             sci::units::length<T> thickness,
             sci::units::temperature<T> temperature = 20_degC)
        : m_length(length), m_width(width), m_thickness(thickness),
          m_temperature(temperature) {}

    // Resistance per unit of length
    auto lineic_resistance() const {
        const auto rho = copper_resistivity(m_temperature);
        return rho / (m_width * m_thickness);
    }

    auto resistance() const {
        const auto R = lineic_resistance() * m_length;
        static_assert(sci::units::is_electrical_resistance<decltype(R)>);
        return R;
    }

    auto voltage_drop(sci::units::electric_current<T> I) const {
        const auto Vdrop = resistance() * I;
        static_assert(sci::units::is_electric_potential<decltype(Vdrop)>);
        return Vdrop;
    }

    sci::units::length<double> m_length;
    sci::units::length<double> m_width;
    sci::units::length<double> m_thickness;
    sci::units::temperature<double> m_temperature;
};

// ----------------------------------------------------------------------------
// Waveguides
// ----------------------------------------------------------------------------

template <class Qty>
constexpr bool is_lineic_capacitance =
    sci::units::is_capacitance<decltype(std::declval<Qty>() * 1_m)>;

template <class Qty>
constexpr bool is_lineic_inductance =
    sci::units::is_inductance<decltype(std::declval<Qty>() * 1_m)>;

template <typename T>
struct Microstrip {
    // https://resources.system-analysis.cadence.com/blog/msa2021-is-there-a-pcb-trace-inductance-rule-of-thumb
    // From IPC-2141
    // height = substrate thickness
    // eps_r = relative dielectric index

    Microstrip(PcbTrace<T> trace, sci::units::length<T> height, T eps_r = T(1))
        : m_trace(trace), m_height(height), m_eps_r(eps_r) {}

    auto impedance() const {
        const auto Z0 =
            87_Ohm *
            sci::log(5.98 * m_height /
                     (0.8 * m_trace.m_width + m_trace.m_thickness)) /
            sci::sqrt(m_eps_r + 1.41);
        static_assert(sci::units::is_electrical_resistance<decltype(Z0)>);
        return Z0;
    }

    // True if formulas for impedance are valid
    bool is_valid_approximation() const {
        return (m_trace.m_width / m_height) <
               sci::units::dimensionless<T>(7.475) -
                   1.25 * (m_trace.m_thickness / m_height);
    }

    auto lineic_capacitance() const {
        constexpr auto C0 = 0.67_pF / 1_in;
        const auto C = C0 * (m_eps_r + 1.41) /
                       sci::log(5.98 * m_height /
                                (0.8 * m_trace.m_width + m_trace.m_thickness));
        static_assert(is_lineic_capacitance<decltype(C)>);
        return C;
    }

    auto capacitance() const {
        const auto C = m_trace.m_length * lineic_capacitance();
        static_assert(sci::units::is_capacitance<decltype(C)>);
        return sci::units::quantity_cast<sci::units::capacitance<T>>(C);
    }

    auto lineic_inductance() const {
        constexpr auto L0 = 5.071_nH / 1_in;
        const auto L =
            L0 * sci::log(5.98 * m_height /
                          (0.8 * m_trace.m_width + m_trace.m_thickness));
        static_assert(is_lineic_inductance<decltype(L)>);
        return L;
    }

    auto inductance() const {
        const auto L = m_trace.m_length * lineic_inductance();
        static_assert(sci::units::is_inductance<decltype(L)>);
        return sci::units::quantity_cast<sci::units::inductance<T>>(L);
    }

    PcbTrace<T> m_trace;
    sci::units::length<T> m_height;
    T m_eps_r;
};

template <typename T>
struct CoaxialCable {
    // https://www.microwaves101.com/encyclopedias/coax

    static constexpr auto eps0 = sci::physical_constants<T>::epsilon0;
    static constexpr auto mu0 = sci::physical_constants<T>::mu0;
    static constexpr auto mu_r = T(1);

    CoaxialCable(sci::units::length<T> D,
                 sci::units::length<T> d,
                 T eps_r = T(1))
        : m_D(D), m_d(d), m_eps_r(eps_r) {}

    auto lineic_capacitance() const {
        const auto C = 2.0 * sci::pi<T> * eps0 * m_eps_r / sci::log(m_D / m_d);
        static_assert(is_lineic_capacitance<decltype(C)>);
        return C;
    }

    auto lineic_inductance() const {
        const auto L = mu0 * mu_r * sci::log(m_D / m_d) / (2.0 * sci::pi<T>);
        static_assert(is_lineic_inductance<decltype(L)>);
        return L;
    }

    auto impedance() const {
        const auto Z0 = sci::sqrt(mu0 * mu_r / (eps0 * m_eps_r)) *
                        sci::log(m_D / m_d) / (2.0 * sci::pi<T>);
        static_assert(sci::units::is_electrical_resistance<decltype(Z0)>);
        return Z0;
    }

    sci::units::length<T> m_D; // Outer shield diameter
    sci::units::length<T> m_d; // Inner conductor diameter
    T m_eps_r;
};

int main() {
    // ----- Power
    std::printf("\n--- Power ---\n");
    constexpr auto U = 2_V;
    constexpr auto I = 3_A;
    constexpr auto P = power(U, I);
    sci::print(sci::units::value(P));

    // ----- Impedance
    std::printf("\n--- Impedance ---\n");
    const auto Z = impedance(50_Ohm, 10_Ohm);
    // const auto Zb = impedance<double>(50., 10.); // Doesn't compile
    const auto V = Z * I;
    static_assert(sci::units::is_electric_potential<decltype(V.real())>);
    static_assert(sci::units::is_electric_potential<decltype(V.imag())>);
    sci::print(sci::units::value(V));

    const auto Zs = impedance(50_Ohm, 0_Ohm);
    const auto Zl = impedance(49_Ohm, 2_Ohm);
    sci::print(reflection_coefficient(Zs, Zl));
    sci::print(vswr(Zs, Zl));
    sci::print(return_loss(Zs, Zl));
    sci::print(is_resistor(Zs));
    sci::print(is_resistor(Zl));
    sci::print(resistance(Zl));
    sci::print(reactance(Zl));
    sci::print(parallel(Zs, Zs));
    sci::print(parallel(Zs, Zs, Zs, Zs));

    const auto Zcapa = get_impedance(1_uF, 1_MHz);
    sci::print(sci::units::value(Zcapa));

    const auto Zind = get_impedance(1_uH, 1_MHz);
    sci::print(sci::units::value(Zind));

    // ----- RLC filter
    std::printf("\n--- RLC filter ---\n");
    const auto R = 0.01_Ohm;
    const auto L = 1_uH;
    const auto C = 1_uF;
    const auto filter = rlc_filter(R, L, C);
    const auto f = sci::logspace<2000, sci::units::frequency<double>>(0.0, 8.0);
    const auto H = filter(f);
    const auto G = sci::absolute(H);
    // const auto phi = sci::angle(H);
    const auto peak_idx = std::size_t(sci::argmax(G));
    const auto peak = 20.0 * sci::log10(G[peak_idx] / G[0]);
    sci::print(peak);
    const auto f_peak = f[peak_idx];
    sci::print(f_peak);
    // Expected peak frequency f = 1 / (2 pi sqrt(LC)) for R = 0
    const auto f_expected = 1.0 / (2.0 * sci::pi<double> * sci::sqrt(L * C));
    static_assert(sci::units::is_frequency<decltype(f_expected)>);
    sci::units::print(f_expected);
    sci::print(f_expected.eval());

    // ----- PCB trace
    std::printf("\n--- PCB trace ---\n");
    const auto rho = copper_resistivity(50_degC);
    sci::print(sci::units::value(rho));

    const auto Length = 10_cm;
    const auto Width = 0.5_mm;
    const auto Thickness = 35_um;
    const auto trace = PcbTrace<double>(Length, Width, Thickness);
    sci::print(sci::units::value(trace.resistance()));
    sci::print(sci::units::value(trace.voltage_drop(4_A)));

    const auto microstrip = Microstrip<double>(trace, 297_um, 4.8);
    sci::print(microstrip.is_valid_approximation());
    sci::print(sci::units::value(microstrip.impedance()));
    sci::print(sci::units::value(microstrip.capacitance()));
    sci::print(sci::units::value(microstrip.inductance()));

    const auto coax = CoaxialCable<double>(1_mm, 0.1_mm, 4.0);
    sci::print(sci::units::value(coax.lineic_capacitance()));
    sci::print(sci::units::value(coax.lineic_inductance()));
    sci::print(sci::units::value(coax.impedance()));
}
