.. _core_inner:

scicpp::inner
====================================

Defined in header <scicpp/core.hpp>

Ordinary inner product of arrays (without complex conjugation).

--------------------------------------

.. function:: template <class InputIt> \
              constexpr auto inner(InputIt first1, InputIt last1, InputIt first2, InputIt last2)

--------------------------------------

.. function:: template <class Array> \
              constexpr auto inner(const Array &a1, const Array &a2)

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.0/reference/generated/numpy.inner.html#numpy.inner>`_