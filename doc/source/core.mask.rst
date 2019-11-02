.. _core_mask:

scicpp::mask
====================================

Defined in header <scicpp/core.hpp>

Return a vector with masked values.

.. function:: template <typename T, std::size_t N, class Mask> \
              std::vector<T> mask(const std::array<T, N> &a, const Mask &m)

--------------------------------------

.. function:: template <typename T, class Mask> \
              std::vector<T> mask(const std::vector<T> &a, const Mask &m)

--------------------------------------

.. function:: template <typename T, class Mask> \
              std::vector<T> mask(std::vector<T> &&a, const Mask &m)

--------------------------------------

:code:`Mask` is an array or vector of booleans, it must be the same size as :code:`a`.

Return a :code:`std::vector` containing only the elements of :code:`a` with a mask index true.
