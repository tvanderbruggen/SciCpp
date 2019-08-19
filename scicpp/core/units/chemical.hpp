// SPDX-License-Identifier: MIT
// Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

// Chemical potential = Energy / Amount of substance
template <typename T, typename Scale = scale<std::ratio<1>>>
using chemical_potential =
    quantity_divide<energy<T, Scale>, amount_of_substance<T>>;

QUANTITY_TRAIT(chemical_potential)