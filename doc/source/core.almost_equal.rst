.. _core_almost_equal:

scicpp::almost_equal
====================================

Defined in header <scicpp/core.hpp>

Compare floating points or arrays of floating points.

Here the type *T* can be either a real or a complex number.

----------------

.. function:: template <int rel_tol = 1, typename T> \
              bool almost_equal(T a, T b)

Return true if the relative distance between *a* and *b* is smaller than
*rel_tol* time epsillon.

----------------

.. function:: template <int rel_tol = 1, typename T, std::size_t N> \
              bool almost_equal(const std::array<T, N> &f1, const std::array<T, N> &f2)

Return true if the relative distance between all the elements of same index 
is smaller than *rel_tol* time epsillon.

----------------

.. function:: template <int rel_tol = 1, typename T> \
              bool almost_equal(const std::vector<T> &f1, const std::vector<T> &f2)

Return true if :code:`f1.size() == f2.size()` anf if the relative distance 
between all the elements of same index is smaller than *rel_tol* time epsillon.