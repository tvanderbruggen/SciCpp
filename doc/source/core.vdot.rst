.. _core_vdot:

scicpp::vdot
====================================

Defined in header <scicpp/core.hpp>

Return the dot product of two vectors.

The vdot(a, b) function handles complex numbers differently than dot(a, b). If the first argument is complex the complex conjugate of the first argument is used for the calculation of the dot product.

--------------------------------------

.. function:: template <class InputItLhs, class InputItRhs> \
              constexpr auto vdot(InputItLhs first1, InputItLhs last1, InputItRhs first2, InputItRhs last2)

--------------------------------------

.. function:: template <class ArrayLhs, class ArrayRhs> \
              constexpr auto vdot(const ArrayLhs &a1, const ArrayRhs &a2)

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.0/reference/generated/numpy.vdot.html#numpy.vdot>`_