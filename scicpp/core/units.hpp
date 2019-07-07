// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_UNITS
#define SCICPP_CORE_UNITS

#include "scicpp/core/meta.hpp"

#include <cstdio>
#include <ratio>
#include <type_traits>

namespace scicpp::units {

// Represents a physical quantity.
// T is the underlying representation type.
// Dim is the dimension of the quantity, that is the rational of primary dimensions.
// Scale is a rational for scaling the quantity.
// TODO: Add Offset for temperature conversions (K <-> Celsius).

template <typename T, typename Dim, typename Scale>
struct quantity;

// is_quantity

namespace detail {

template <class T>
struct is_quantity : std::false_type {};

template <typename T, typename Dim, typename Scale>
struct is_quantity<quantity<T, Dim, Scale>> : std::true_type {};

} // namespace detail

template <class T>
inline constexpr bool is_quantity_v = detail::is_quantity<T>::value;

template <class T>
using enable_if_is_quantity = std::enable_if_t<is_quantity_v<T>, int>;

template <class T>
using disable_if_is_quantity = std::enable_if_t<!is_quantity_v<T>, int>;

// common_quantity

template <typename T, typename Dim, typename Scale1, typename Scale2>
struct common_quantity {
  private:
    static constexpr auto gcd_num = std::gcd(Scale1::num, Scale2::num);
    static constexpr auto gcd_den = std::gcd(Scale1::den, Scale2::den);
    using CommonScale =
        typename std::ratio<gcd_num, (Scale1::den / gcd_den) * Scale2::den>;

  public:
    using type = quantity<T, Dim, CommonScale>;
};

template <typename T, typename Dim, typename Scale1, typename Scale2>
using common_quantity_t =
    typename common_quantity<T, Dim, Scale1, Scale2>::type;

// quantity_cast

template <typename ToQty,
          typename T,
          typename Dim,
          typename Scale,
          enable_if_is_quantity<ToQty> = 0>
auto quantity_cast(const quantity<T, Dim, Scale> &qty) {
    constexpr auto conversion_factor =
        (T(ToQty::scale::den) / T(ToQty::scale::num)) *
        (T(Scale::num) / T(Scale::den));
    return ToQty(qty.value() * conversion_factor);
}

template <typename T, typename Dim, typename Scale>
struct quantity {
    static_assert(meta::is_ratio_v<Dim>);
    static_assert(meta::is_ratio_v<Scale>);
    static_assert(Dim::num > 0);
    static_assert(Scale::num > 0);

    using value_type = T;
    using dim = Dim;
    using scale = Scale;

    // Constructors, destructors, copy

    constexpr quantity() = default;

    quantity(const quantity &) = default;

    explicit constexpr quantity(T value) : m_value(value) {}

    template <typename Scale2>
    constexpr quantity(const quantity<T, Dim, Scale2> &qty)
        : m_value(quantity_cast<quantity<T, Dim, Scale>>(qty).value()) {}

    ~quantity() = default;

    quantity &operator=(const quantity &) = default;

    // Comparisons

    template <typename RhsScale>
    constexpr auto operator==(const quantity<T, Dim, RhsScale> &rhs) const {
        constexpr auto factor1 = T(Scale::num) * T(RhsScale::den);
        constexpr auto factor2 = T(RhsScale::num) * T(Scale::den);
        return factor1 * m_value == factor2 * rhs.value();
    }

    template <typename RhsScale>
    constexpr auto operator!=(const quantity<T, Dim, RhsScale> &rhs) const {
        return !(*this == rhs);
    }

    template <typename RhsScale>
    constexpr auto operator<(const quantity<T, Dim, RhsScale> &rhs) const {
        constexpr auto factor1 = T(Scale::num) * T(RhsScale::den);
        constexpr auto factor2 = T(RhsScale::num) * T(Scale::den);
        return factor1 * m_value < factor2 * rhs.value();
    }

    template <typename RhsScale>
    constexpr auto operator<=(const quantity<T, Dim, RhsScale> &rhs) const {
        return !(rhs < *this);
    }

    template <typename RhsScale>
    constexpr auto operator>(const quantity<T, Dim, RhsScale> &rhs) const {
        return rhs < *this;
    }

    template <typename RhsScale>
    constexpr auto operator>=(const quantity<T, Dim, RhsScale> &rhs) const {
        return !(*this < rhs);
    }

    // Arithmetic

    constexpr auto operator+=(const quantity<T, Dim, Scale> &rhs) {
        m_value += rhs.m_value;
        return *this;
    }

    constexpr auto operator-=(const quantity<T, Dim, Scale> &rhs) {
        m_value -= rhs.m_value;
        return *this;
    }

    constexpr auto operator+() const { return *this; }

    template <typename RhsScale>
    constexpr auto operator+(const quantity<T, Dim, RhsScale> &rhs) const {
        using cq = common_quantity_t<T, Dim, Scale, RhsScale>;
        return cq(cq(*this).value() + cq(rhs).value());
    }

    constexpr auto operator-() const {
        return quantity<T, Dim, Scale>(-m_value);
    }

    template <typename RhsScale>
    constexpr auto operator-(const quantity<T, Dim, RhsScale> &rhs) const {
        using cq = common_quantity_t<T, Dim, Scale, RhsScale>;
        return cq(cq(*this).value() - cq(rhs).value());
    }

    template <typename RhsDim, typename RhsScale>
    constexpr auto operator*(const quantity<T, RhsDim, RhsScale> &rhs) const {
        return quantity<T,
                        typename std::ratio_multiply<Dim, RhsDim>::type,
                        typename std::ratio_multiply<Scale, RhsScale>::type>(
            m_value * rhs.value());
    }

    template <typename RhsDim, typename RhsScale>
    constexpr auto operator/(const quantity<T, RhsDim, RhsScale> &rhs) const {
        return quantity<T,
                        typename std::ratio_divide<Dim, RhsDim>::type,
                        typename std::ratio_divide<Scale, RhsScale>::type>(
            m_value / rhs.value());
    }

    constexpr auto inv() const {
        return quantity<T,
                        typename std::ratio_divide<std::ratio<1>, Dim>::type,
                        typename std::ratio_divide<std::ratio<1>, Scale>::type>(
            T{1} / m_value);
    }

    constexpr auto value() const { return m_value; }

  protected:
    T m_value = T{0};
};

template <typename T1,
          typename T2,
          typename Dim,
          typename Scale,
          meta::disable_if_iterable<T1> = 0>
constexpr auto operator*(T1 factor, const quantity<T2, Dim, Scale> &rhs) {
    // using T = std::common_type_t<T1, T2>;
    using T = decltype(std::declval<T1>() * std::declval<T2>());
    return quantity<T, Dim, Scale>(T(factor) * T(rhs.value()));
}

template <typename T1,
          typename T2,
          typename Dim,
          typename Scale,
          meta::disable_if_iterable<T2> = 0>
constexpr auto operator*(const quantity<T1, Dim, Scale> &rhs, T2 factor) {
    return factor * rhs;
}

template <typename T1,
          typename T2,
          typename Dim,
          typename Scale,
          meta::disable_if_iterable<T1> = 0>
constexpr auto operator/(T1 factor, const quantity<T2, Dim, Scale> &rhs) {
    return factor * rhs.inv();
}

template <typename T1,
          typename T2,
          typename Dim,
          typename Scale,
          meta::disable_if_iterable<T2> = 0>
constexpr auto operator/(const quantity<T1, Dim, Scale> &rhs, T2 factor) {
    return rhs * (T2{1} / factor);
}

template <typename T, typename Dim, typename Scale>
void print(const quantity<T, Dim, Scale> &q) {
    std::printf("%Lf x %lu / %lu [%lu, %lu]\n",
                q.value(),
                Scale::num,
                Scale::den,
                Dim::num,
                Dim::den);
}

template <typename Quantity1, typename Quantity2>
using quantity_multiply =
    decltype(std::declval<Quantity1>() * std::declval<Quantity2>());

template <typename Quantity1, typename Quantity2>
using quantity_divide =
    decltype(std::declval<Quantity1>() / std::declval<Quantity2>());

template <typename Quantity>
using quantity_invert = decltype(std::declval<Quantity>().inv());

namespace primary_flags {

// Each base quantity is identified by a prime number.
// using Dimensionless = std::ratio<1>;

// SI
using Length = std::ratio<2>;
using Time = std::ratio<3>;
using Mass = std::ratio<5>;
using Current = std::ratio<7>;
using Temperature = std::ratio<9>;
using AmountOfSubstance = std::ratio<11>;
using LuminousIntensity = std::ratio<13>;

// Data

using DataQuantity = std::ratio<17>;

} // namespace primary_flags

// Primary quantities

template <typename T, typename Scale = std::ratio<1>>
using length = quantity<T, primary_flags::Length, Scale>;

template <typename T, typename Scale = std::ratio<1>>
using time = quantity<T, primary_flags::Time, Scale>;

template <typename T, typename Scale = std::ratio<1>>
using mass = quantity<T, primary_flags::Mass, Scale>;

template <typename T, typename Scale = std::ratio<1>>
using current = quantity<T, primary_flags::Current, Scale>;

template <typename T, typename Scale = std::ratio<1>>
using temperature = quantity<T, primary_flags::Temperature, Scale>;

template <typename T, typename Scale = std::ratio<1>>
using amount_of_substance =
    quantity<T, primary_flags::AmountOfSubstance, Scale>;

template <typename T, typename Scale = std::ratio<1>>
using luminous_intensity = quantity<T, primary_flags::LuminousIntensity, Scale>;

template <typename T, typename Scale = std::ratio<1>>
using data_quantity = quantity<T, primary_flags::DataQuantity, Scale>;

// Derived quantities

template <typename T, typename Scale = std::ratio<1>>
using speed = quantity_divide<length<T, Scale>, time<T>>;

template <typename T, typename Scale = std::ratio<1>>
using acceleration = quantity_divide<speed<T, Scale>, time<T>>;

template <typename T, typename Scale = std::ratio<1>>
using surface = quantity_multiply<length<T, Scale>, length<T>>;

template <typename T, typename Scale = std::ratio<1>>
using volume = quantity_multiply<surface<T, Scale>, length<T>>;

// Force = Mass x Acceleration
template <typename T, typename Scale = std::ratio<1>>
using force = quantity_multiply<mass<T, Scale>, acceleration<T>>;

// Power = Force x Speed
template <typename T, typename Scale = std::ratio<1>>
using power = quantity_multiply<force<T, Scale>, speed<T>>;

// Energy = Power x Time
template <typename T, typename Scale = std::ratio<1>>
using energy = quantity_multiply<power<T, Scale>, time<T>>;

template <typename T, typename Scale = std::ratio<1>>
using pressure = quantity_divide<force<T, Scale>, surface<T>>;

template <typename T, typename Scale = std::ratio<1>>
using frequency = quantity_invert<time<T, Scale>>;

// Resistance = Power / Current^2
template <typename T, typename Scale = std::ratio<1>>
using resistance =
    quantity_divide<power<T, Scale>, quantity_multiply<current<T>, current<T>>>;

template <typename T, typename Scale = std::ratio<1>>
using conductivity = quantity_invert<resistance<T, Scale>>;

// Voltage = Power / Current
template <typename T, typename Scale = std::ratio<1>>
using voltage = quantity_divide<power<T, Scale>, current<T>>;

// Capacitance = Time / Resistance
template <typename T, typename Scale = std::ratio<1>>
using capacitance = quantity_divide<time<T, Scale>, resistance<T>>;

// Charge = Capacitance x Voltage
template <typename T, typename Scale = std::ratio<1>>
using charge = quantity_multiply<capacitance<T, Scale>, voltage<T>>;

// Inductance = Voltage x Time / Current
template <typename T, typename Scale = std::ratio<1>>
using inductance =
    quantity_divide<quantity_multiply<voltage<T, Scale>, time<T>>, current<T>>;

// Magnetic field = Force / (Charge x Speed)
template <typename T, typename Scale = std::ratio<1>>
using magnetic_field =
    quantity_divide<force<T, Scale>, quantity_multiply<charge<T>, speed<T>>>;

template <typename T, typename Scale = std::ratio<1>>
using data_rate = quantity_divide<data_quantity<T, Scale>, time<T>>;

namespace literals {

// ---------- Length

constexpr auto operator"" _fm(long double femtometers) {
    return length<long double, std::femto>{femtometers};
}

constexpr auto operator"" _pm(long double picometers) {
    return length<long double, std::pico>{picometers};
}

constexpr auto operator"" _nm(long double nanometers) {
    return length<long double, std::nano>{nanometers};
}

constexpr auto operator"" _um(long double micrometers) {
    return length<long double, std::micro>{micrometers};
}

constexpr auto operator"" _mm(long double meters) {
    return length<long double, std::milli>{meters};
}

constexpr auto operator"" _cm(long double centimeters) {
    return length<long double, std::centi>{centimeters};
}

constexpr auto operator"" _m(long double meters) {
    return length<long double>{meters};
}

constexpr auto operator"" _km(long double meters) {
    return length<long double, std::kilo>{meters};
}

constexpr auto operator"" _angstrom(long double angstroms) {
    return length<long double, std::ratio<1, 10000000000>>{angstroms};
}

constexpr auto operator"" _in(long double inches) {
    return length<long double, std::ratio<254, 10000>>{inches};
}

constexpr auto operator"" _ft(long double inches) {
    return length<long double, std::ratio<3048, 10000>>{inches};
}

constexpr auto operator"" _yd(long double yards) {
    return length<long double, std::ratio<9144, 10000>>{yards};
}

// ---------- Surface

constexpr auto operator"" _m2(long double squaremeters) {
    return surface<long double>{squaremeters};
}

constexpr auto operator"" _km2(long double kilosquaremeters) {
    return surface<long double, std::mega>{kilosquaremeters};
}

constexpr auto operator"" _ha(long double hectares) {
    return surface<long double, std::ratio<10000, 1>>{hectares};
}

// ---------- Volume

constexpr auto operator"" _m3(long double cubicmeters) {
    return volume<long double>{cubicmeters};
}

// ---------- Time

constexpr auto operator"" _ms(long double milliseconds) {
    return time<long double, std::milli>{milliseconds};
}

constexpr auto operator"" _s(long double seconds) {
    return time<long double>{seconds};
}

constexpr auto operator"" _min(long double minutes) {
    return time<long double, std::ratio<60, 1>>{minutes};
}

constexpr auto operator"" _h(long double hours) {
    return time<long double, std::ratio<3600, 1>>{hours};
}

// ---------- Speed

constexpr auto operator"" _m_per_s(long double meters_per_seconds) {
    return speed<long double>{meters_per_seconds};
}

constexpr auto operator"" _km_per_h(long double kilometers_per_hours) {
    return speed<long double, std::ratio<3600, 1000>>{kilometers_per_hours};
}

// ---------- Mass

constexpr auto operator"" _g(long double grams) {
    return mass<long double, std::milli>{grams};
}

constexpr auto operator"" _kg(long double kilograms) {
    return mass<long double>{kilograms};
}

// ---------- Energy

constexpr auto operator"" _J(long double joules) {
    return energy<long double>{joules};
}

constexpr auto operator"" _kJ(long double kilojoules) {
    return energy<long double, std::kilo>{kilojoules};
}

constexpr auto operator"" _MJ(long double megajoules) {
    return energy<long double, std::mega>{megajoules};
}

constexpr auto operator"" _kWh(long double kilowatthours) {
    return energy<long double, std::ratio<3600000, 1>>{kilowatthours};
}

constexpr auto operator"" _cal(long double calories) {
    return energy<long double, std::ratio<41855, 10000>>{calories};
}

// ---------- Current

constexpr auto operator"" _mA(long double milliamperes) {
    return current<long double, std::milli>{milliamperes};
}

constexpr auto operator"" _A(long double amperes) {
    return current<long double>{amperes};
}

// ---------- Temperature

constexpr auto operator"" _K(long double kelvins) {
    return temperature<long double>{kelvins};
}

// ---------- Power

constexpr auto operator"" _mW(long double milliwatts) {
    return power<long double, std::milli>{milliwatts};
}

constexpr auto operator"" _W(long double watts) {
    return power<long double>{watts};
}

constexpr auto operator"" _kW(long double kilowatts) {
    return power<long double, std::kilo>{kilowatts};
}

// ---------- Voltage

constexpr auto operator"" _mV(long double millivolts) {
    return voltage<long double, std::milli>{millivolts};
}

constexpr auto operator"" _V(long double volts) {
    return voltage<long double>{volts};
}

// ---------- Frequency

constexpr auto operator"" _Hz(long double hertzs) {
    return frequency<long double>{hertzs};
}

// ---------- Magnetic field

constexpr auto operator"" _mT(long double milliteslas) {
    return magnetic_field<long double, std::milli>{milliteslas};
}

constexpr auto operator"" _T(long double teslas) {
    return magnetic_field<long double>{teslas};
}

constexpr auto operator"" _G(long double gauss) {
    return magnetic_field<long double, std::ratio<1, 10000>>{gauss};
}

// ---------- Inductance

constexpr auto operator"" _H(long double henrys) {
    return inductance<long double>{henrys};
}

// ---------- Capacitance

constexpr auto operator"" _F(long double farads) {
    return capacitance<long double>{farads};
}

// ---------- Charge

constexpr auto operator"" _C(long double coulombs) {
    return charge<long double>{coulombs};
}

// ---------- Pressure

constexpr auto operator"" _Pa(long double pascals) {
    return pressure<long double>{pascals};
}

constexpr auto operator"" _kPa(long double kilopascals) {
    return pressure<long double, std::kilo>{kilopascals};
}

constexpr auto operator"" _bar(long double bars) {
    return pressure<long double, std::ratio<100000>>{bars};
}

constexpr auto operator"" _mmHg(long double mmhg) {
    return pressure<long double, std::ratio<101325, 760>>{mmhg};
}

constexpr auto operator"" _torr(long double torr) {
    return pressure<long double, std::ratio<101325, 760>>{torr};
}

constexpr auto operator"" _psi(long double psi) {
    return pressure<long double, std::ratio<689476, 100>>{psi};
}

constexpr auto operator"" _atm(long double atmospheres) {
    return pressure<long double, std::ratio<101325>>{atmospheres};
}

// ---------- Quantity of data

constexpr auto operator"" _b(unsigned long long bits) {
    return data_quantity<unsigned long long>{bits};
}

constexpr auto operator"" _B(unsigned long long bytes) {
    return data_quantity<unsigned long long, std::ratio<8>>{bytes};
}

constexpr auto operator"" _kiB(unsigned long long kilobytes) {
    return data_quantity<unsigned long long, std::ratio<8LL * 1024LL>>{
        kilobytes};
}

constexpr auto operator"" _MiB(unsigned long long megabytes) {
    return data_quantity<unsigned long long, std::ratio<8LL * 1024LL * 1024LL>>{
        megabytes};
}

constexpr auto operator"" _GiB(unsigned long long gigabytes) {
    return data_quantity<unsigned long long,
                         std::ratio<8LL * 1024LL * 1024LL * 1024LL>>{gigabytes};
}

constexpr auto operator"" _TiB(unsigned long long terabytes) {
    return data_quantity<unsigned long long,
                         std::ratio<8LL * 1024LL * 1024LL * 1024LL * 1024LL>>{
        terabytes};
}

// ---------- Data rates

constexpr auto operator"" _bps(long double bit_per_seconds) {
    return data_rate<long double>{bit_per_seconds};
}

} // namespace literals

} // namespace scicpp::units

#endif // SCICPP_CORE_UNITS