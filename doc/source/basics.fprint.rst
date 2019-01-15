.. _basics_fprint:

scicpp::fprint
====================================

Defined in header <scicpp/print.hpp>

Print an array to a file stream.

----------------

.. function:: template <typename T, std::size_t N> \
              void fprint(FILE *stream, const std::array<T, N> &A)

----------------

.. function:: template <typename T> \
              void fprint(FILE *stream, const std::vector<T> &V)
