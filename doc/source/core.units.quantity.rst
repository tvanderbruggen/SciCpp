.. _core_units_quantity:

scicpp::units::quantity
====================================

Defined in header <scicpp/core.hpp>

----------------

.. class:: template <typename T, typename Dim, typename Scale, typename Offset = std::ratio<0>> quantity

An arithmetic-like type representing a quantity. The class only stores a value of type `T`.

It performs compile-time `Dimensional analysis <https://en.wikipedia.org/wiki/Dimensional_analysis>`_ to validate Dimensional homogeneity.
The quantity dimension Dim is a compile-time rational constant representing the quantity dimension(for example L^2 T / L).

It provides conversion between units (for example meters to miles), whith conversion factor calculated at compile-time.
Scale and Offset are compile-time rational constant representing the affine transform coefficients to convert between units.

Operators
-------------------------

Arithmetic operators :code:`++`, :code:`--`, :code:`+`, :code:`-`, :code:`*`, :code:`/`