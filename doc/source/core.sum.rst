.. _core_sum:

scicpp::sum
====================================

Defined in header <scicpp/core.hpp>

Sum of array elements.

Standard functions
-------------------------

.. function:: template <class InputIt> \
              constexpr auto sum(InputIt first, InputIt last)

--------------------------------------

.. function:: template <class Array> \
              constexpr auto sum(const Array &f)

--------------------------------------

With filtering predicate
-------------------------

These variants accept a predicate to specify values that must be filtered in.

These functions return a pair with first the sum result, and then the number of elements used to compute the sum.

--------------------------------------

.. function:: template <class InputIt, class Predicate> \
              constexpr auto sum(InputIt first, InputIt last, Predicate filter)

--------------------------------------

.. function:: template <class Array, class Predicate> \
              constexpr auto sum(const Array &f, Predicate filter)

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.1/reference/generated/numpy.sum.html#numpy.sum>`_