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

- Assignment operator: :code:`=`
- Arithmetic operators: :code:`++`, :code:`--`, :code:`+=`, :code:`-=`, :code:`*=`, :code:`/=`, :code:`+`, :code:`-`, :code:`*`, :code:`/`
- Comparison operators: :code:`==`, :code:`!=`, :code:`>=`, :code:`<=`, :code:`>`, :code:`<`

Additions, substractions and comparisons can only be performed between quantities of same dimension :code:`Dim`.

Member functions
-------------------------

.. function:: constexpr T value() const

Return the underlying value of the quantity (ex. :code:`1_km.eval()` returns 1.0).
Should mostly be used for printing.

.. function:: constexpr T eval() const

Return the value of a quantity evaluated in the base unit (ex. :code:`1_km.eval()` returns 1000.0).
Should mostly be used for printing.

Non-member functions
-------------------------

.. function:: template <typename ToQuantity, typename T, typename Dim, typename Scale, typename Offset> \
              constexpr ToQuantity quantity_cast(const quantity<T, Dim, Scale, Offset> &qty)

Convert `qty` to a quantity of type `ToQuantity`.
Used to convert between units when implicit conversion is not allowed, that is if it would result in a loss of precision.

Type traits
-------------------------

.. class:: template <class T> is_quantity_v

Return :code:`true` if :code:`T` is a quantity.
For example, :code:`is_quantity_v<meter<>> == true`, but :code:`is_quantity_v<double> == false`.

-------------------------

.. class:: template <typename T, typename Dim, typename Scale1, typename Scale2, typename Offset1, typename Offset2> common_quantity_t

Determines the common type of two units.

-------------------------

.. class:: template <class T> representation_t

Return the representation type is :code:`T` is a quantity, else simply return :code:`T`.

For example, :code:`representation_t<meter<int>>` is :code:`int` and :code:`representation_t<double>` is :code:`double`.

-------------------------

Quantity types arithmetic
~~~~~~~~~~~~~~~~~~~~~~~~~

.. class:: template <typename Quantity1, typename Quantity2> quantity_multiply

.. class:: template <typename Quantity1, typename Quantity2> quantity_divide

.. class:: template <typename Quantity> quantity_invert

.. class:: template <intmax_t Root, typename Quantity> quantity_root

Use to build derived new quantities from existing ones:

::

    // Energy = Power x Time
    template <typename T, typename Scale>
    using energy = quantity_multiply<power<T, Scale>, time<T>>;

    // Pressure = Force / Area
    template <typename T, typename Scale>
    using pressure = quantity_divide<force<T, Scale>, area<T>>;

    // Frequency = 1 / Time
    template <typename T, typename Scale>
    using frequency = quantity_invert<time<T, Scale>>;

    // Voltage noise density = Electric Potential / sqrt(Bandwidth)
    template <typename T, typename Scale>
    using voltage_noise_density = quantity_divide<electric_potential<T, Scale>,
                                                  quantity_root<2, frequency<T>>>;
