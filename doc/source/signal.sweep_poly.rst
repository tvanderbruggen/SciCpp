.. _signal_sweep_poly:

scicpp::signal::sweep_poly
====================================

Defined in header <scicpp/signal.hpp>

Frequency-swept cosine generator, with a time-dependent frequency.

--------------------------------------

.. function:: template <class Array, class Poly> \
              Array sweep_poly(Array &&t, const Poly &poly, T phi = 0)

:t: Times at which to evaluate the waveform (:code:`std::array` or :code:`std::vector`).
   
:poly: The desired frequency expressed as a polynomial (:code:`std::array` or :code:`std::vector`).

       |warning| Polynomial coefficients in order of increasing degree (i.e., :expr:`{1., 2., 3.}` is :math:`1 + 2 X + 3 X^{2}`).

:phi: Phase offset, in degrees

Return an array of the same length as :code:`t`.

--------------------------------------

See also
    ----------
    `Scipy documentation <https://docs.scipy.org/doc/scipy-1.2.0/reference/generated/scipy.signal.sweep_poly.html#scipy.signal.sweep_poly>`_

.. |warning| unicode:: U+26A0