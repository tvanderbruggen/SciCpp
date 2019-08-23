// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

// Chemical potential = Energy / Amount of substance
template <typename T, typename Scale = scale<std::ratio<1>>>
using chemical_potential =
    quantity_divide<energy<T, Scale>, amount_of_substance<T>>;

QUANTITY_TRAIT(chemical_potential)

// Molarity = Amount of substance / Volume
template <typename T, typename Scale = scale<std::ratio<1>>>
using molarity =
    quantity_divide<amount_of_substance<T, Scale>, volume<T>>;

QUANTITY_TRAIT(molarity)

// Molality = Amount of substance / Mass
template <typename T, typename Scale = scale<std::ratio<1>>>
using molality =
    quantity_divide<amount_of_substance<T, Scale>, mass<T>>;

QUANTITY_TRAIT(molality)

// Catalytic activity = Amount of substance / Time
template <typename T, typename Scale = scale<std::ratio<1>>>
using catalytic_activity =
    quantity_divide<amount_of_substance<T, Scale>, time<T>>;

QUANTITY_TRAIT(catalytic_activity)