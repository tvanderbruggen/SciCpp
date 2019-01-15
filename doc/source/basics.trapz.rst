.. _basics_trapz:

scicpp::trapz
====================================

Defined in header <scicpp/numeric.hpp>

Integrate using the trapezoidal rule.

--------------------------------------

.. function:: template <class InputIt> \
              constexpr T trapz(InputIt first, InputIt last, T dx)

Integrate over the elements between a pair of iterators.

:dx: Integration step (spacing between sample points).

--------------------------------------

.. function:: template <typename T, std::size_t N> \
              constexpr T trapz(const std::array<T, N> &f, T dx)

Integrate over the elements in a std::array.

--------------------------------------

.. function:: template <typename T> \
              constexpr T trapz(const std::vector<T> &f, T dx)

Integrate over the elements in a std::vector.

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy/reference/generated/numpy.prod.html>`_