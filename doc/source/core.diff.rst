.. _core_diff:

scicpp::diff
====================================

Defined in header <scicpp/core.hpp>

The n-th discrete difference between consecutive elements of an array.

--------------------------------------

.. function:: template <int n = 1, typename T, std::size_t N> \
              std::array<T, max(N - n, 0)> diff(const std::array<T, N> &a)

--------------------------------------

.. function:: template <typename T> \
              std::vector<T> diff(const std::vector<T> &a, int n = 1)

.. function:: template <typename T> \
              std::vector<T> diff(std::vector<T> &&a, int n = 1)

Return a std::vector of size :code:`max(a.size() - n, 0)`.

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/numpy-1.15.0/reference/generated/numpy.diff.html#numpy.diff>`_