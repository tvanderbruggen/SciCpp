.. _core_nansum:

scicpp::nansum
====================================

Defined in header <scicpp/core.hpp>

Sum of array elements excluding NaNs.

--------------------------------------

.. function:: template <typename T, std::size_t N> \
              T nansum(const std::array<T, N> &f)

--------------------------------------

.. function:: template <typename T> \
              T nansum(const std::vector<T> &f)

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.0/reference/generated/numpy.nansum.html#numpy.nansum>`_