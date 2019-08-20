// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

// Voltage = Power / Current
template <typename T, typename Scale = scale<std::ratio<1>>>
using electric_potential =
    quantity_divide<power<T, Scale>, electric_current<T>>;

QUANTITY_TRAIT(electric_potential)

// Charge = Current x Time
template <typename T, typename Scale = scale<std::ratio<1>>>
using electric_charge = quantity_multiply<electric_current<T, Scale>, time<T>>;

QUANTITY_TRAIT(electric_charge)

// Electrical resistance = Power / Current^2
template <typename T, typename Scale = scale<std::ratio<1>>>
using electrical_resistance = quantity_divide<
    power<T, Scale>,
    quantity_multiply<electric_current<T>, electric_current<T>>>;

QUANTITY_TRAIT(electrical_resistance)

// Electrical conductivity = 1 / Electrical resistance
template <typename T, typename Scale = scale<std::ratio<1>>>
using electrical_conductivity =
    quantity_invert<electrical_resistance<T, Scale>>;

QUANTITY_TRAIT(electrical_conductivity)

// Capacitance = Time / Resistance
template <typename T, typename Scale = scale<std::ratio<1>>>
using capacitance = quantity_divide<time<T, Scale>, electrical_resistance<T>>;

QUANTITY_TRAIT(capacitance)

// Inductance = Voltage x Time / Current
template <typename T, typename Scale = scale<std::ratio<1>>>
using inductance =
    quantity_divide<quantity_multiply<electric_potential<T, Scale>, time<T>>,
                    electric_current<T>>;

QUANTITY_TRAIT(inductance)

// Magnetic field = Force / (Charge x Speed)
template <typename T, typename Scale = scale<std::ratio<1>>>
using magnetic_flux_density =
    quantity_divide<force<T, Scale>,
                    quantity_multiply<electric_charge<T>, speed<T>>>;

QUANTITY_TRAIT(magnetic_flux_density)

// Magnetic flux = Magnetic field / Area
template <typename T, typename Scale = scale<std::ratio<1>>>
using magnetic_flux =
    quantity_multiply<magnetic_flux_density<T, Scale>, area<T>>;

QUANTITY_TRAIT(magnetic_flux)

template <typename T, typename Scale = scale<std::ratio<1>>>
using current_noise_density =
    quantity_divide<electric_current<T, Scale>, quantity_root<2, frequency<T>>>;

QUANTITY_TRAIT(current_noise_density)

template <typename T, typename Scale = scale<std::ratio<1>>>
using voltage_noise_density = quantity_divide<electric_potential<T, Scale>,
                                              quantity_root<2, frequency<T>>>;

QUANTITY_TRAIT(voltage_noise_density)

// ----------------------------------------------------------------------------
// Electric potential
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(electric_potential, volt)

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(electric_potential, _fV, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(electric_potential, _pV, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(electric_potential, _nV, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(electric_potential, _uV, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(electric_potential, _mV, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(electric_potential, _V, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(electric_potential, _kV, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL(electric_potential, _MV, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL(electric_potential, _GV, std::giga)
SCICPP_CORE_UNITS_SET_LITERAL(electric_potential, _TV, std::tera)

} // namespace literals

// ----------------------------------------------------------------------------
// Electric charge
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(electric_charge, coulomb)

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(electric_charge, _fC, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(electric_charge, _pC, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(electric_charge, _nC, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(electric_charge, _uC, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(electric_charge, _mC, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(electric_charge, _C, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(electric_charge, _kC, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL(electric_charge, _MC, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL(electric_charge, _GC, std::giga)
SCICPP_CORE_UNITS_SET_LITERAL(electric_charge, _TC, std::tera)

} // namespace literals

// ----------------------------------------------------------------------------
// Electrical resistance
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(electrical_resistance, ohm)

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(electrical_resistance, _fOhm, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(electrical_resistance, _pOhm, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(electrical_resistance, _nOhm, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(electrical_resistance, _uOhm, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(electrical_resistance, _mOhm, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(electrical_resistance, _Ohm, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(electrical_resistance, _kOhm, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL(electrical_resistance, _MOhm, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL(electrical_resistance, _GOhm, std::giga)
SCICPP_CORE_UNITS_SET_LITERAL(electrical_resistance, _TOhm, std::tera)

} // namespace literals

// ----------------------------------------------------------------------------
// Electrical conductivity
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(electrical_conductivity, siemens)

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(electrical_conductivity, _fS, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(electrical_conductivity, _pS, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(electrical_conductivity, _nS, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(electrical_conductivity, _uS, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(electrical_conductivity, _mS, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(electrical_conductivity, _S, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(electrical_conductivity, _kS, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL(electrical_conductivity, _MS, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL(electrical_conductivity, _GS, std::giga)
SCICPP_CORE_UNITS_SET_LITERAL(electrical_conductivity, _TS, std::tera)

} // namespace literals

// ----------------------------------------------------------------------------
// Capacitance
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(capacitance, farad)

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(capacitance, _fF, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(capacitance, _pF, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(capacitance, _nF, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(capacitance, _uF, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(capacitance, _mF, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(capacitance, _F, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(capacitance, _kF, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL(capacitance, _MF, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL(capacitance, _GF, std::giga)
SCICPP_CORE_UNITS_SET_LITERAL(capacitance, _TF, std::tera)

} // namespace literals

// ----------------------------------------------------------------------------
// Inductance
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(inductance, henry)

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(inductance, _fH, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(inductance, _pH, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(inductance, _nH, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(inductance, _uH, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(inductance, _mH, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(inductance, _H, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(inductance, _kH, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL(inductance, _MH, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL(inductance, _GH, std::giga)
SCICPP_CORE_UNITS_SET_LITERAL(inductance, _TH, std::tera)

} // namespace literals

// ----------------------------------------------------------------------------
// Magnetic flux density
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(magnetic_flux_density, tesla)

template <typename T = double>
using gauss = magnetic_flux_density<T, scale<std::ratio<1, 10000>>>;

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(magnetic_flux_density, _fT, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(magnetic_flux_density, _pT, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(magnetic_flux_density, _nT, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(magnetic_flux_density, _uT, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(magnetic_flux_density, _mT, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(magnetic_flux_density, _T, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL_RATIO(magnetic_flux_density, _G, 1, 10000)

} // namespace literals

// ----------------------------------------------------------------------------
// Magnetic flux
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(magnetic_flux, weber)

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(magnetic_flux, _fWb, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(magnetic_flux, _pWb, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(magnetic_flux, _nWb, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(magnetic_flux, _uWb, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(magnetic_flux, _mWb, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(magnetic_flux, _Wb, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(magnetic_flux, _kWb, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL(magnetic_flux, _MWb, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL(magnetic_flux, _GWb, std::giga)
SCICPP_CORE_UNITS_SET_LITERAL(magnetic_flux, _TWb, std::tera)

} // namespace literals

// ----------------------------------------------------------------------------
// Current noise density
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(current_noise_density,
                                        ampere_per_root_hertz)

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(current_noise_density, _fA_per_rtHz, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(current_noise_density, _pA_per_rtHz, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(current_noise_density, _nA_per_rtHz, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(current_noise_density, _uA_per_rtHz, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(current_noise_density, _mA_per_rtHz, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(current_noise_density, _A_per_rtHz, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(current_noise_density, _kA_per_rtHz, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL(current_noise_density, _MA_per_rtHz, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL(current_noise_density, _GA_per_rtHz, std::giga)
SCICPP_CORE_UNITS_SET_LITERAL(current_noise_density, _TA_per_rtHz, std::tera)

} // namespace literals

// ----------------------------------------------------------------------------
// Voltage noise density
// ----------------------------------------------------------------------------

SCICPP_CORE_UNITS_DEFINE_PREFIXES_ALIAS(voltage_noise_density,
                                        volt_per_root_hertz)

namespace literals {

SCICPP_CORE_UNITS_SET_LITERAL(voltage_noise_density, _fV_per_rtHz, std::femto)
SCICPP_CORE_UNITS_SET_LITERAL(voltage_noise_density, _pV_per_rtHz, std::pico)
SCICPP_CORE_UNITS_SET_LITERAL(voltage_noise_density, _nV_per_rtHz, std::nano)
SCICPP_CORE_UNITS_SET_LITERAL(voltage_noise_density, _uV_per_rtHz, std::micro)
SCICPP_CORE_UNITS_SET_LITERAL(voltage_noise_density, _mV_per_rtHz, std::milli)
SCICPP_CORE_UNITS_SET_LITERAL(voltage_noise_density, _V_per_rtHz, std::ratio<1>)
SCICPP_CORE_UNITS_SET_LITERAL(voltage_noise_density, _kV_per_rtHz, std::kilo)
SCICPP_CORE_UNITS_SET_LITERAL(voltage_noise_density, _MV_per_rtHz, std::mega)
SCICPP_CORE_UNITS_SET_LITERAL(voltage_noise_density, _GV_per_rtHz, std::giga)
SCICPP_CORE_UNITS_SET_LITERAL(voltage_noise_density, _TV_per_rtHz, std::tera)

} // namespace literals