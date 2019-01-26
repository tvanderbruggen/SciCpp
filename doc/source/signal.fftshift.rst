.. _signal_fftshift:

scicpp::signal::fftshift
====================================

Defined in header <scicpp/signal.hpp>

Shift the zero-frequency component to the center of the spectrum.

In place Shift
---------------------------

--------------------------------------

.. function:: template <typename T, std::size_t N> \
              void fftshift_inplace(std::array<T, N> &a)

--------------------------------------

.. function:: template <typename T> \
              void fftshift_inplace(std::vector<T> &v)

Out of place Shift
---------------------------

--------------------------------------

.. function:: template <typename T, std::size_t N> \
              constexpr std::array<T, N> fftshift(const std::array<T, N> &a)

--------------------------------------

.. function:: template <typename T> \
              std::vector<T> fftshift(const std::vector<T> &v)