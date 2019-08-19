// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_UNITS_UNITS
#define SCICPP_CORE_UNITS_UNITS

#include "scicpp/core/meta.hpp"
#include "scicpp/core/units/arithmetic.hpp"
#include "scicpp/core/units/quantity.hpp"

#include <cmath>
#include <cstdint>
#include <cstdio>
#include <numeric>
#include <ratio>
#include <type_traits>

namespace scicpp::units {

using DimSystem = dimensional_system<11>;

// ----------------------------------------------------------------------------
// Quantities
// ----------------------------------------------------------------------------

// Base quantities

template <typename T, typename Scale = scale<std::ratio<1>>>
using dimensionless = get_base_quantity<0, DimSystem, T, Scale>;

template <typename T, typename Scale = scale<std::ratio<1>>>
using length = get_base_quantity<1, DimSystem, T, Scale>;

template <typename T, typename Scale = scale<std::ratio<1>>>
using time = get_base_quantity<2, DimSystem, T, Scale>;

template <typename T, typename Scale = scale<std::ratio<1>>>
using mass = get_base_quantity<3, DimSystem, T, Scale>;

template <typename T, typename Scale = scale<std::ratio<1>>>
using electric_current = get_base_quantity<4, DimSystem, T, Scale>;

template <typename T,
          typename Scale = scale<std::ratio<1>>,
          typename Offset = std::ratio<0>>
using temperature = get_base_quantity<5, DimSystem, T, Scale, Offset>;

template <typename T, typename Scale = scale<std::ratio<1>>>
using amount_of_substance = get_base_quantity<6, DimSystem, T, Scale>;

template <typename T, typename Scale = scale<std::ratio<1>>>
using luminous_intensity = get_base_quantity<7, DimSystem, T, Scale>;

template <typename T, typename Scale = scale<std::ratio<1>>>
using planar_angle = get_base_quantity<8, DimSystem, T, Scale>;

template <typename T, typename Scale = scale<std::ratio<1>>>
using solid_angle = get_base_quantity<9, DimSystem, T, Scale>;

template <typename T, typename Scale = scale<std::ratio<1>>>
using data_quantity = get_base_quantity<10, DimSystem, T, Scale>;

// Derived quantities

// Speed = Length / Time
template <typename T, typename Scale = scale<std::ratio<1>>>
using speed = quantity_divide<length<T, Scale>, time<T>>;

// Acceleration = Speed / Time
template <typename T, typename Scale = scale<std::ratio<1>>>
using acceleration = quantity_divide<speed<T, Scale>, time<T>>;

// Angular velocity = Planar angle / Time
template <typename T, typename Scale = scale<std::ratio<1>>>
using angular_velocity = quantity_divide<planar_angle<T, Scale>, time<T>>;

// Momentum = Mass x Speed
template <typename T, typename Scale = scale<std::ratio<1>>>
using momentum = quantity_multiply<mass<T, Scale>, speed<T>>;

// Area = Length x Length
template <typename T, typename Scale = scale<std::ratio<1>>>
using area = quantity_multiply<length<T, Scale>, length<T>>;

// Volume = Area x Length
template <typename T, typename Scale = scale<std::ratio<1>>>
using volume = quantity_multiply<area<T, Scale>, length<T>>;

// Force = Mass x Acceleration
template <typename T, typename Scale = scale<std::ratio<1>>>
using force = quantity_multiply<mass<T, Scale>, acceleration<T>>;

// Power = Force x Speed
template <typename T, typename Scale = scale<std::ratio<1>>>
using power = quantity_multiply<force<T, Scale>, speed<T>>;

// Energy = Power x Time
template <typename T, typename Scale = scale<std::ratio<1>>>
using energy = quantity_multiply<power<T, Scale>, time<T>>;

template <typename T, typename Scale = scale<std::ratio<1>>>
using work = energy<T, Scale>;

// Pressure = Force / Area
template <typename T, typename Scale = scale<std::ratio<1>>>
using pressure = quantity_divide<force<T, Scale>, area<T>>;

// Frequency = 1 / Time
template <typename T, typename Scale = scale<std::ratio<1>>>
using frequency = quantity_invert<time<T, Scale>>;

// Data rate = Quantity of data / Time
template <typename T, typename Scale = scale<std::ratio<1>>>
using data_rate = quantity_divide<data_quantity<T, Scale>, time<T>>;

// Quantities traits

#define QUANTITY_TRAIT(qty)                                                    \
    template <class T>                                                         \
    constexpr bool is_##qty##_impl() {                                         \
        if constexpr (is_quantity_v<T>) {                                      \
            return is_same_dimension<                                          \
                T,                                                             \
                qty<typename T::value_type, typename T::scal>>;                \
        } else {                                                               \
            return false;                                                      \
        }                                                                      \
    }                                                                          \
                                                                               \
    template <class T>                                                         \
    inline constexpr bool is_##qty = is_##qty##_impl<T>();

QUANTITY_TRAIT(dimensionless)
QUANTITY_TRAIT(length)
QUANTITY_TRAIT(time)
QUANTITY_TRAIT(mass)
QUANTITY_TRAIT(electric_current)
QUANTITY_TRAIT(temperature)
QUANTITY_TRAIT(amount_of_substance)
QUANTITY_TRAIT(luminous_intensity)
QUANTITY_TRAIT(planar_angle)
QUANTITY_TRAIT(solid_angle)
QUANTITY_TRAIT(data_quantity)

QUANTITY_TRAIT(speed)
QUANTITY_TRAIT(acceleration)
QUANTITY_TRAIT(angular_velocity)
QUANTITY_TRAIT(momentum)
QUANTITY_TRAIT(area)
QUANTITY_TRAIT(volume)
QUANTITY_TRAIT(force)
QUANTITY_TRAIT(power)
QUANTITY_TRAIT(energy)
QUANTITY_TRAIT(work)
QUANTITY_TRAIT(pressure)
QUANTITY_TRAIT(frequency)

// ----------------------------------------------------------------------------
// Units
//
// An unit is the specialization of a quantity
// template for a given scale and offset.
// ----------------------------------------------------------------------------

#define SCICPP_CORE_UNITS_SET_LITERAL(quantity, literal, scale_ratio)          \
    constexpr auto operator""##literal(long double x) {                        \
        return quantity<double, scale<scale_ratio>>{static_cast<double>(x)};   \
    }                                                                          \
                                                                               \
    constexpr auto operator""##literal(unsigned long long x) {                 \
        return quantity<double, scale<scale_ratio>>{static_cast<double>(x)};   \
    }

#define SCICPP_CORE_UNITS_SET_LITERAL_RATIO(quantity, literal, num, den)       \
    constexpr auto operator""##literal(long double x) {                        \
        return quantity<double, scale<std::ratio<(num), (den)>>>{              \
            static_cast<double>(x)};                                           \
    }                                                                          \
                                                                               \
    constexpr auto operator""##literal(unsigned long long x) {                 \
        return quantity<double, scale<std::ratio<(num), (den)>>>{              \
            static_cast<double>(x)};                                           \
    }

#define SCICPP_CORE_UNITS_SET_LITERAL_RATIO2(                                  \
    quantity, literal, num1, den1, num2, den2)                                 \
    constexpr auto operator""##literal(long double x) {                        \
        return quantity<double,                                                \
                        scale<std::ratio<(num1), (den1)>>,                     \
                        std::ratio<(num2), (den2)>>{static_cast<double>(x)};   \
    }                                                                          \
                                                                               \
    constexpr auto operator""##literal(unsigned long long x) {                 \
        return quantity<double,                                                \
                        scale<std::ratio<(num1), (den1)>>,                     \
                        std::ratio<(num2), (den2)>>{static_cast<double>(x)};   \
    }

#define SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(quantity, base_unit)           \
    template <typename T = double>                                             \
    using atto##base_unit = quantity<T, scale<std::atto>>;                     \
    template <typename T = double>                                             \
    using femto##base_unit = quantity<T, scale<std::femto>>;                   \
    template <typename T = double>                                             \
    using pico##base_unit = quantity<T, scale<std::pico>>;                     \
    template <typename T = double>                                             \
    using nano##base_unit = quantity<T, scale<std::nano>>;                     \
    template <typename T = double>                                             \
    using micro##base_unit = quantity<T, scale<std::micro>>;                   \
    template <typename T = double>                                             \
    using milli##base_unit = quantity<T, scale<std::milli>>;                   \
    template <typename T = double>                                             \
    using centi##base_unit = quantity<T, scale<std::centi>>;                   \
    template <typename T = double>                                             \
    using deci##base_unit = quantity<T, scale<std::deci>>;                     \
    template <typename T = double>                                             \
    using base_unit = quantity<T>;                                             \
    template <typename T = double>                                             \
    using deca##base_unit = quantity<T, scale<std::deca>>;                     \
    template <typename T = double>                                             \
    using hecto##base_unit = quantity<T, scale<std::hecto>>;                   \
    template <typename T = double>                                             \
    using kilo##base_unit = quantity<T, scale<std::kilo>>;                     \
    template <typename T = double>                                             \
    using mega##base_unit = quantity<T, scale<std::mega>>;                     \
    template <typename T = double>                                             \
    using giga##base_unit = quantity<T, scale<std::giga>>;                     \
    template <typename T = double>                                             \
    using tera##base_unit = quantity<T, scale<std::tera>>;                     \
    template <typename T = double>                                             \
    using peta##base_unit = quantity<T, scale<std::peta>>;                     \
    template <typename T = double>                                             \
    using exa##base_unit = quantity<T, scale<std::exa>>;

// Primary quantities

// ----------------------------------------------------------------------------
// Length
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(length, meter)

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(length, _fm, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(length, _pm, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(length, _nm, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(length, _um, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(length, _mm, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(length, _cm, std::centi)
SCICPP_CORE_UNITS_SET_LITERAL(length, _m, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(length, _km, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL(length, _Mm, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL(length, _Gm, std::giga)
SCICPP_CORE_UNITS_SET_LITERAL(length, _Tm, std::tera)

SCICPP_CORE_UNITS_SET_LITERAL_RATIO(length, _angstrom, 1, 10000000000)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO(length, _in, 254, 10000)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO(length, _ft, 3048, 10000)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO(length, _yd, 9144, 10000)

} // namespace literals

// ----------------------------------------------------------------------------
// Time
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(time, second)

template <typename T = double>
using minute = time<T, scale<std::ratio<60>>>;

template <typename T = double>
using hour = time<T, scale<std::ratio<3600>>>;

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(time, _fs, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(time, _ps, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(time, _ns, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(time, _us, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(time, _ms, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(time, _s, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO(time, _min, 60, 1)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO(time, _h, 3600, 1)

} // namespace literals

// ----------------------------------------------------------------------------
// Mass
// ----------------------------------------------------------------------------

template <typename T = double>
using gram = mass<T, scale<std::milli>>;

template <typename T = double>
using kilogram = mass<T>;

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(mass, _ng, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(mass, _ug, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(mass, _mg, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(mass, _g, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(mass, _kg, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(mass, _ton, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL(mass, _kton, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL(mass, _Mton, std::giga)
SCICPP_CORE_UNITS_SET_LITERAL(mass, _Gton, std::tera)
SCICPP_CORE_UNITS_SET_LITERAL(mass, _Tton, std::exa)

} // namespace literals

// ----------------------------------------------------------------------------
// Electric current
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(electric_current, ampere)

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(electric_current, _fA, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(electric_current, _pA, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(electric_current, _nA, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(electric_current, _uA, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(electric_current, _mA, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(electric_current, _A, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(electric_current, _kA, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL(electric_current, _MA, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL(electric_current, _GA, std::giga)
SCICPP_CORE_UNITS_SET_LITERAL(electric_current, _TA, std::tera)

} // namespace literals

// ----------------------------------------------------------------------------
// Temperature
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(temperature, kelvin)

template <typename T = double>
using celsius = temperature<T, scale<std::ratio<1>>, std::ratio<27315, 100>>;

template <typename T = double>
using fahrhenheit =
    temperature<T, scale<std::ratio<5, 9>>, std::ratio<45967, 180>>;

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(temperature, _fK, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(temperature, _pK, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(temperature, _nK, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(temperature, _uK, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(temperature, _mK, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(temperature, _K, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO2(temperature, _degC, 1, 1, 27315, 100)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO2(temperature, _degF, 5, 9, 45967, 180)

} // namespace literals

// ----------------------------------------------------------------------------
// Amount of substance
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(amount_of_substance, mole)

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(amount_of_substance, _fmol, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(amount_of_substance, _pmol, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(amount_of_substance, _nmol, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(amount_of_substance, _umol, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(amount_of_substance, _mmol, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(amount_of_substance, _mol, std::ratio<1>)

} // namespace literals

// ----------------------------------------------------------------------------
// Luminous intensity
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(luminous_intensity, candela)

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(luminous_intensity, _fCd, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(luminous_intensity, _pCd, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(luminous_intensity, _nCd, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(luminous_intensity, _uCd, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(luminous_intensity, _mCd, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(luminous_intensity, _Cd, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(luminous_intensity, _kCd, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL(luminous_intensity, _MCd, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL(luminous_intensity, _GCd, std::giga)

} // namespace literals

// ----------------------------------------------------------------------------
// Planar angle
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(planar_angle, radian)

using pi_ratio = std::ratio<21053343141, 6701487259>;
using deg_to_rad_ratio = std::ratio_divide<pi_ratio, std::ratio<180>>;

template <typename T = double>
using degree = planar_angle<T, scale<deg_to_rad_ratio>>;

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(planar_angle, _frad, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(planar_angle, _prad, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(planar_angle, _nrad, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(planar_angle, _urad, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(planar_angle, _mrad, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(planar_angle, _rad, std::ratio<1>)

SCICPP_CORE_UNITS_SET_LITERAL_RATIO(planar_angle,
                                    _deg,
                                    deg_to_rad_ratio::num,
                                    deg_to_rad_ratio::den)

} // namespace literals

// ----------------------------------------------------------------------------
// Data quantity
// ----------------------------------------------------------------------------

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(data_quantity, _b, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO(data_quantity, _B, 8, 1)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO(data_quantity, _kiB, 8LL * 1024LL, 1)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO(data_quantity,
                                    _MiB,
                                    8LL * 1024LL * 1024LL,
                                    1)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO(data_quantity,
                                    _GiB,
                                    8LL * 1024LL * 1024LL * 1024LL,
                                    1)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO(data_quantity,
                                    _TiB,
                                    8LL * 1024LL * 1024LL * 1024LL * 1024LL,
                                    1)

} // namespace literals

// ----------------------------------------------------------------------------
// Speed
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(speed, meter_per_second)

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(speed, _fm_per_s, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(speed, _pm_per_s, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(speed, _nm_per_s, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(speed, _um_per_s, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(speed, _mm_per_s, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(speed, _m_per_s, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(speed, _km_per_s, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO(speed, _km_per_h, 3600, 1000)

} // namespace literals

// ----------------------------------------------------------------------------
// Acceleration
// ----------------------------------------------------------------------------

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(acceleration, _fm_per_s2, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(acceleration, _pm_per_s2, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(acceleration, _nm_per_s2, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(acceleration, _um_per_s2, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(acceleration, _mm_per_s2, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(acceleration, _m_per_s2, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(acceleration, _km_per_s2, std::kilo)

} // namespace literals

// ----------------------------------------------------------------------------
// area
// ----------------------------------------------------------------------------

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(area, _m2, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(area, _km2, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO(area, _ha, 10000, 1)

} // namespace literals

// ----------------------------------------------------------------------------
// Volume
// ----------------------------------------------------------------------------

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(volume, _m3, std::ratio<1>)

} // namespace literals

// ----------------------------------------------------------------------------
// Force
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(force, newton)

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(force, _fN, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(force, _pN, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(force, _nN, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(force, _uN, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(force, _mN, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(force, _N, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(force, _kN, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL(force, _MN, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL(force, _GN, std::giga)
SCICPP_CORE_UNITS_SET_LITERAL(force, _TN, std::tera)

} // namespace literals

// ----------------------------------------------------------------------------
// Power
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(power, watt)

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(power, _fW, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(power, _pW, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(power, _nW, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(power, _uW, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(power, _mW, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(power, _W, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(power, _kW, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL(power, _MW, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL(power, _GW, std::giga)
SCICPP_CORE_UNITS_SET_LITERAL(power, _TW, std::tera)

} // namespace literals

// ----------------------------------------------------------------------------
// Energy
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(energy, joule)

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(energy, _fJ, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(energy, _pJ, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(energy, _nJ, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(energy, _uJ, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(energy, _mJ, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(energy, _J, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(energy, _kJ, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL(energy, _MJ, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL(energy, _GJ, std::giga)
SCICPP_CORE_UNITS_SET_LITERAL(energy, _TJ, std::tera)

SCICPP_CORE_UNITS_SET_LITERAL_RATIO(energy, _kWh, 3600000, 1)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO(energy, _cal, 41855, 10000)

} // namespace literals

// ----------------------------------------------------------------------------
// Pressure
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(pressure, pascal)

template <typename T = double>
using bar = pressure<T, scale<std::ratio<100000>>>;

template <typename T = double>
using mmHg = pressure<T, scale<std::ratio<101325, 760>>>;

template <typename T = double>
using torr = mmHg<T>;

template <typename T = double>
using psi = pressure<T, scale<std::ratio<689476, 100>>>;

template <typename T = double>
using atm = pressure<T, scale<std::ratio<101325>>>;

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(pressure, _fPa, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(pressure, _pPa, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(pressure, _nPa, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(pressure, _uPa, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(pressure, _mPa, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(pressure, _Pa, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(pressure, _kPa, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL(pressure, _MPa, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL(pressure, _GPa, std::giga)
SCICPP_CORE_UNITS_SET_LITERAL(pressure, _TPa, std::tera)

SCICPP_CORE_UNITS_SET_LITERAL_RATIO(pressure, _bar, 100000, 1)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO(pressure, _mmHg, 101325, 760)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO(pressure, _torr, 101325, 760)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO(pressure, _psi, 689476, 100)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO(pressure, _atm, 101325, 1)

} // namespace literals

// ----------------------------------------------------------------------------
// Frequency
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(frequency, hertz)

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(frequency, _fHz, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(frequency, _pHz, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(frequency, _nHz, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(frequency, _uHz, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(frequency, _mHz, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(frequency, _Hz, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(frequency, _kHz, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL(frequency, _MHz, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL(frequency, _GHz, std::giga)
SCICPP_CORE_UNITS_SET_LITERAL(frequency, _THz, std::tera)

} // namespace literals

// ----------------------------------------------------------------------------
// Data rate
// ----------------------------------------------------------------------------

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(data_rate, _bps, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(data_rate, _kbps, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL(data_rate, _Mbps, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL(data_rate, _Gbps, std::giga)
SCICPP_CORE_UNITS_SET_LITERAL(data_rate, _Tbps, std::tera)

} // namespace literals

#include "chemical.hpp"
#include "electromagnetism.hpp"
#include "photometry.hpp"
#include "thermal.hpp"

#undef QUANTITY_TRAIT
#undef SCICPP_CORE_UNITS_SET_LITERAL
#undef SCICPP_CORE_UNITS_SET_LITERAL_RATIO
#undef SCICPP_CORE_UNITS_SET_LITERAL_RATIO2
#undef SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS

} // namespace scicpp::units

#endif // SCICPP_CORE_UNITS_UNITS