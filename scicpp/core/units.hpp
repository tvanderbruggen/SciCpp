// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

#ifndef SCICPP_CORE_UNITS
#define SCICPP_CORE_UNITS

#include "scicpp/core/numeric.hpp"

#include <ratio>

namespace scicpp {

template <typename T, typename UnitFlag>
struct BaseUnit {
    static_assert(UnitFlag::num > 0);

    using unit_flag = UnitFlag;

    BaseUnit(T value) : m_value(value) {}

    // Comparisons

    auto operator==(const BaseUnit<T, UnitFlag> rhs) const {
        return m_value == rhs.m_value;
    }

    auto operator!=(const BaseUnit<T, UnitFlag> rhs) const {
        return !(*this == rhs);
    }

    template <int rel_tol = 1>
    auto is_approx(const BaseUnit<T, UnitFlag> rhs) const {
        return almost_equal<rel_tol>(m_value, rhs.m_value);
    }

    auto operator>=(const BaseUnit<T, UnitFlag> rhs) const {
        return m_value >= rhs.m_value;
    }

    auto operator<=(const BaseUnit<T, UnitFlag> rhs) const {
        return m_value <= rhs.m_value;
    }

    auto operator>(const BaseUnit<T, UnitFlag> rhs) const {
        return m_value > rhs.m_value;
    }

    auto operator<(const BaseUnit<T, UnitFlag> rhs) const {
        return m_value < rhs.m_value;
    }

    // Arithmetic

    auto operator+=(const BaseUnit<T, UnitFlag> rhs) {
        m_value += rhs.m_value;
        return *this;
    }

    auto operator-=(const BaseUnit<T, UnitFlag> rhs) {
        m_value -= rhs.m_value;
        return *this;
    }

    auto operator+(const BaseUnit<T, UnitFlag> rhs) const {
        return BaseUnit<T, UnitFlag>(m_value + rhs.m_value);
    }

    auto operator-(const BaseUnit<T, UnitFlag> rhs) const {
        return BaseUnit<T, UnitFlag>(m_value - rhs.m_value);
    }

    template <typename RhsUnitFlag>
    auto operator*(const BaseUnit<T, RhsUnitFlag> rhs) const {
        return BaseUnit<T,
                        std::ratio<UnitFlag::num * RhsUnitFlag::num,
                                   UnitFlag::den * RhsUnitFlag::den>>(
            m_value * rhs.value());
    }

    template <typename RhsUnitFlag>
    auto operator/(const BaseUnit<T, RhsUnitFlag> rhs) const {
        return BaseUnit<T,
                        std::ratio<UnitFlag::num * RhsUnitFlag::den,
                                   UnitFlag::den * RhsUnitFlag::num>>(
            m_value / rhs.value());
    }

    auto invert() const {
        return BaseUnit<T, std::ratio<UnitFlag::den, UnitFlag::num>>(T{1} /
                                                                     m_value);
    }

    auto value() const { return m_value; }

  protected:
    T m_value = T{0};
};

namespace unit_flags {

// Base units
using Meter = std::ratio<1>;
using Second = std::ratio<2>;
using Kilogram = std::ratio<3>;
using Ampere = std::ratio<4>;
using Kelvin = std::ratio<5>;
using Mole = std::ratio<6>;
using Candela = std::ratio<7>;

// Derived units
using Volt = std::ratio<8>;
using Ohm = std::ratio<9>;
using Newton = std::ratio<10>;

} // namespace unit_flags

template <typename T>
using meter = BaseUnit<T, unit_flags::Meter>;

template <typename T>
using second = BaseUnit<T, unit_flags::Second>;

template <typename T>
using kilogram = BaseUnit<T, unit_flags::Kilogram>;

template <typename T>
using ampere = BaseUnit<T, unit_flags::Ampere>;

template <typename T>
using kelvin = BaseUnit<T, unit_flags::Kelvin>;

template <typename T>
using mole = BaseUnit<T, unit_flags::Mole>;

template <typename T>
using candela = BaseUnit<T, unit_flags::Candela>;

template <typename T>
using volt = BaseUnit<T, unit_flags::Volt>;

template <typename T>
using ohm = BaseUnit<T, unit_flags::Ohm>;

template <typename T>
using newton = BaseUnit<T, unit_flags::Newton>;

template <typename T>
using watt = decltype(std::declval<volt<T>> * std::declval<ampere<T>>);

template <typename T>
using joule = decltype(std::declval<watt<T>> * std::declval<second<T>>);

template <typename T>
using pascal = decltype(std::declval<newton<T>> /
                        (std::declval<meter<T>> * std::declval<meter<T>>));

} // namespace scicpp

#endif // SCICPP_CORE_UNITS