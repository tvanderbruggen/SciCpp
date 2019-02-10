.. _core_cumprod:

scicpp::cumprod
====================================

Defined in header <scicpp/core.hpp>

Cumulative product of array elements.

--------------------------------------

.. function:: template <typename T, std::size_t N> \
              std::array<T, N> cumprod(const std::array<T, N> &a)

--------------------------------------

.. function:: template <typename T> \
              std::vector<T> cumprod(const std::vector<T> &a)

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.0/reference/generated/numpy.cumsum.html>`_