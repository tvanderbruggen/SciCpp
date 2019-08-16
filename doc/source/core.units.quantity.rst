.. _core_units_quantity:

scicpp::units::quantity
====================================

Defined in header <scicpp/core.hpp>

----------------

.. class:: template <typename T, typename Dim, typename Scale, typename Offset = std::ratio<0>> quantity

An arithmetic-like type representing a quantity. The class only stores a value of type `T`.

It performs compile-time `Dimensional analysis <https://en.wikipedia.org/wiki/Dimensional_analysis>`_ to validate Dimensional homogeneity.
The quantity dimension Dim is a compile-time constant representing the quantity dimension (for example L^2 T / L).
See :ref:`Implementation notes <core_units_implementation_notes>`.

It also provides conversion between units (for example meters to miles), whith conversion factor calculated at compile-time.
Scale and Offset are compile-time rational constant representing the affine transform coefficients to convert between units.

Operators
-------------------------

- Asignment operator: :code:`=`
- Arithmetic operators: :code:`++`, :code:`--`, :code:`+=`, :code:`-=`, :code:`*=`, :code:`/=`, :code:`+`, :code:`-`, :code:`*`, :code:`/`
- Comparison operators: :code:`==`, :code:`!=`, :code:`>=`, :code:`<=`, :code:`>`, :code:`<`

Member functions
-------------------------

.. function:: constexpr T value() const

Return the underlying value of the quantity.
Should mostly be used for printing.

Non-member functions
-------------------------

.. function:: template <typename ToQuantity, typename T, typename Dim, typename Scale, typename Offset> \
              constexpr ToQuantity quantity_cast(const quantity<T, Dim, Scale, Offset> &qty)

Convert `qty` to a quantity of type `ToQuantity`.
Used to convert between units when implicit conversion is not allowed, that is if it would result in a loss of precision.
