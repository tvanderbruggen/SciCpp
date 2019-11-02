.. _core_mask_array:

scicpp::mask_array
====================================

Defined in header <scicpp/core.hpp>

Mask a vector in-place.

.. function:: template <typename T, class Mask> \
              void mask_array(std::vector<T> &a, const Mask &m)

:code:`Mask` is an array or vector of booleans, it must be the same size as :code:`a`.