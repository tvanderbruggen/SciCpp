.. _signal_zero_padding:

scicpp::signal::zero_padding
====================================

Defined in header <scicpp/signal/fft.hpp>

Performs zero padding of an array.

--------------------------------------

.. function::  template <typename T> \
               std::vector<T> zero_padding(const std::vector<T> &v, std::size_t new_size)

if *new_size* is larger than :code:`v.size()` then zero are added the vector end,
else the vector is cropped.