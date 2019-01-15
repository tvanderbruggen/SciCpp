.. _basics_sum:

scicpp::sum
====================================

Defined in header <scicpp/numeric.hpp>

Sum of array elements.

--------------------------------------

.. function:: template <class InputIt> \
              constexpr T sum(InputIt first, InputIt last)

Compute the sum of the elements between a pair of iterators.

--------------------------------------

.. function:: template <typename T, std::size_t N> \
              constexpr T sum(const std::array<T, N> &f)

Compute the sum of the elements in a std::array.

--------------------------------------

.. function:: template <typename T> \
              constexpr T sum(const std::vector<T> &f)

Compute the sum of the elements in a std::vector.

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.1/reference/generated/numpy.sum.html#numpy.sum>`_