.. _core_units_implementation_notes:

Units implementation notes
====================================

The International System of Units defines the system for physical quantities with the base quantities
length, mass, time, electric current, thermodynamic temperature, amount of substance, luminous intensity.
But this system is not necessary complete, we may want to use more base quantities such as angles,
amount of data, currency, telephony offered load (Erlang) ...

Here we present how the Units modules implements a generic dimensional analysis for a system of units.

Dimensional system
-----------------------

A dimensional system is derived from a set of **base quantities** :math:`\{ q_{i} \}_{1 \leq i \leq N}`.
Any quantity can be written as:

.. math::

    D = \prod_{i=1}^{N} q_{i}^{\alpha_{i}}, \; \alpha_{i} \in \mathbb{R}.

That is a new quantity is derived by multiplying and/or dividing other quantities.

Note that a fractionnal power is possible, for example the square-root of a quantity is possible.

Dimension representation
-----------------------

Here we implement :math:`\alpha_{i} \in \mathbb{Q}`, thus approximating a real number by a rational number.

Let :math:`\alpha_{i} = n_{i} / d_{i}`, with :math:`n_{i} \in \mathbb{Z}`, :math:`d_{i} \in \mathbb{N}^{*}` and :math:`\gcd(n_{i}, d_{i}) = 1`, then

.. math::

    D = \left[ \prod_{i=1}^{N} q_{i}^{m_{i}} \right]^{1/R},

where :math:`m_{i} = n_{i} \prod_{j \neq i} d_{j} \in \mathbb{Z}` and :math:`R = \prod_{i=1}^{N} d_{i} \in \mathbb{N}^{*}`.

To each base quantity :math:`q_{i}` we associate an unique **prime number** :math:`p_{i}`.
The quantity :math:`D` can thus be represented as 

.. math::

    D \equiv \left[ \prod_{i=1}^{N} p_{i}^{m_{i}} \right]^{1/R} = (P/Q)^{1/R},

where :math:`P/Q \in \mathbb{Q}`. Therefore, a dimension can be modelled as the **root of a ratio** and a set of three integers: :math:`\mathrm{Dim} \langle P, Q, R \rangle`.

The base quantities content of :math:`\mathrm{Dim} \langle P, Q, R \rangle` can be recovered by performing a prime factor decompostion of :math:`P` and :math:`Q`. 

A dimension representation :math:`\mathrm{Dim} \langle P, Q, R \rangle` is **irreducible** iff :math:`\gcd(P, Q) = 1` and :math:`Q` and :math:`P` are not perfect :math:`R^{\mathrm{th}}` powers
(`Wikipedia <https://en.wikipedia.org/wiki/Perfect_power>`_).

Two quantities are identical if they have the same irreducible dimension representation.

NB: This provides a compact and unique signature (given the correspondance :math:`q_{i} \mapsto p_{i}` is well define), that could be use in communication protocols to transfer data units.

Dimension multiplication
-----------------------

.. math::

    \mathrm{Dim} \langle P_{1}, Q_{1}, R_{1} \rangle \times \mathrm{Dim} \langle P_{2}, Q_{2}, R_{2} \rangle 
    = \left( \frac{P_{1}}{Q_{1}} \right)^{1/R_{1}} \times \left( \frac{P_{2}}{Q_{2}} \right)^{1/R_{2}}
    = \left( \frac{P_{1}^{R_{2}}}{Q_{1}^{R_{2}}} \frac{P_{2}^{R_{1}}}{Q_{2}^{R_{1}}} \right)^{1/(R_{1} R_{2})}
    = \mathrm{Dim} \langle P_{1}^{R_{2}} P_{2}^{R_{1}}, Q_{1}^{R_{2}} Q_{2}^{R_{1}}, R_{1} R_{2} \rangle.
