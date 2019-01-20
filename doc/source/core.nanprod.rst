.. _core_nanprod:

scicpp::nanprod
====================================

Defined in header <scicpp/core.hpp>

Product of array elements excluding NaNs.

--------------------------------------

.. function:: template <typename T, std::size_t N> \
              T nanprod(const std::array<T, N> &f)

--------------------------------------

.. function:: template <typename T> \
              T nanprod(const std::vector<T> &f)

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.0/reference/generated/numpy.nanprod.html#numpy.nanprod>`_