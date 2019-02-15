.. _core_prod:

scicpp::prod
====================================

Defined in header <scicpp/core.hpp>

Product of array elements.

Standard functions
-------------------------

.. function:: template <class InputIt> \
              constexpr auto prod(InputIt first, InputIt last)

--------------------------------------

.. function:: template <class Array> \
              constexpr auto prod(const Array &f)

--------------------------------------

With filtering predicate
-------------------------

These variants accept a predicate to specify values that must be filtered in.

These functions return a pair with first the sum result, and then the number of elements used to compute the sum.

--------------------------------------

.. function:: template <class InputIt, class Predicate> \
              constexpr auto prod(InputIt first, InputIt last, Predicate filter)

--------------------------------------

.. function:: template <class Array, class Predicate> \
              constexpr auto prod(const Array &f, Predicate filter)

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy/reference/generated/numpy.prod.html>`_