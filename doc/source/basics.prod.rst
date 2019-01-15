.. _basics_prod:

scicpp::prod
====================================

Defined in header <scicpp/numeric.hpp>

Product of array elements.

--------------------------------------

.. function:: template <class InputIt> \
              constexpr T prod(InputIt first, InputIt last)

Compute the product of the elements between a pair of iterators.

--------------------------------------

.. function:: template <typename T, std::size_t N> \
              constexpr T prod(const std::array<T, N> &f)

Compute the product of the elements in a std::array.

--------------------------------------

.. function:: template <typename T> \
              constexpr T prod(const std::vector<T> &f)

Compute the product of the elements in a std::vector.

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy/reference/generated/numpy.prod.html>`_