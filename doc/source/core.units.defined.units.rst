.. _core_units_defined_units:

Defined units
====================================

Defined in header <scicpp/core.hpp>

----------------

Here :code:`T` is the representation type (:code:`double` by default). 

The statement :code:`using namespace scicpp::units::literals` must be included in the scope where
unit literals are used.

Base units
----------------------

-------------------------------------------------------------------------------

Length
~~~~~~~~~~~~~~~

SI units
^^^^^^^^^^

:Classes: :code:`attometer<T>, femtometer<T>, picometer<T>, nanometer<T>, micrometer<T>, millimeter<T>, centimeter<T>, decimeter<T>, meter<T>, decameter<T>, hectometer<T>, kilometer<T>, megameter<T>, gigameter<T>, terameter<T>, petameter<T>, exameter<T>`.

:Literals: :code:`_am, _fm, _pm, _nm, _um, _mm, _cm, _m, _km, _Mm, _Gm, _Tm, _Pm, _Em`.

Non-SI units
^^^^^^^^^^

Imperial

:Classes: :code:`inch<T>, foot<T>, yard<T>, mile<T>`.

:Literals: :code:`_in, _ft, _yd, _mi`.

Marine

:Classes: :code:`nautical_mile<T>`.

:Literals: :code:`_nmi`.

Astronomy

:Classes: :code:`astronomical_unit<T>, light_year<T>, parsec<T>`.

:Literals: :code:`_au, _ly, _pc`.

Miscellaneous

:Classes: :code:`angstrom<T>`.

:Literals: :code:`_angstrom`.

-------------------------------------------------------------------------------

Time
~~~~~~~~~~~~~~~

SI units
^^^^^^^^^^

:Classes: :code:`attosecond<T>, femtosecond<T>, picosecond<T>, nanosecond<T>, microsecond<T>, millisecond<T>, centisecond<T>, decisecond<T>, second<T>, decasecond<T>, hectosecond<T>, kilosecond<T>, megasecond<T>, gigasecond<T>, terasecond<T>, petasecond<T>, exasecond<T>`.

:Literals: :code:`_as, _fs, _ps, _ns, _us, _ms, _s, _ks, _Ms, _Gs, _Ts, _Ps, _Es`.

Non-SI units
^^^^^^^^^^

:Classes: :code:`minute<T>, hour<T>, day<T>, week<T>`.

:Literals: :code:`_min, _h, _day, _week`.

-------------------------------------------------------------------------------

Mass
~~~~~~~~~~~~~~~

SI units
^^^^^^^^^^

:Classes: :code:`femtogram<T>, picogram<T>, nanogram<T>, microgram<T>, milligram<T>, gram<T>, kilogram<T>`.

:Literals: :code:`_fg, _pg, _ng, _ug, _mg, _g, _kg`.

Non-SI units
^^^^^^^^^^

:Classes: :code:`tonne<T>, kilotonne<T>, megatonne<T>, gigatonne<T>, teratonne<T>`.

:Literals: :code:`_t, _kt, _Mt, _Gt, _Tt`.

-------------------------------------------------------------------------------

Electric current
~~~~~~~~~~~~~~~

SI units
^^^^^^^^^^

:Classes: :code:`attoampere<T>, femtoampere<T>, picoampere<T>, nanoampere<T>, microampere<T>, milliampere<T>, centiampere<T>, deciampere<T>, ampere<T>, decaampere<T>, hectoampere<T>, kiloampere<T>, megaampere<T>, gigaampere<T>, teraampere<T>, petaampere<T>, exaampere<T>`.

:Literals: :code:`_aA, _fA, _pA, _nA, _uA, _mA, _A, _kA, _MA, _GA, _TA, _PA, _EA`.

-------------------------------------------------------------------------------

Temperature
~~~~~~~~~~~~~~~

SI units
^^^^^^^^^^

:Classes: :code:`attokelvin<T>, femtokelvin<T>, picokelvin<T>, nanokelvin<T>, microkelvin<T>, millikelvin<T>, centikelvin<T>, decikelvin<T>, kelvin<T>, decakelvin<T>, hectokelvin<T>, kilokelvin<T>, megakelvin<T>, gigakelvin<T>, terakelvin<T>, petakelvin<T>, exakelvin<T>`.

:Literals: :code:`_aK, _fK, _pK, _nK, _uK, _mK, _K, _kK, _MK, _GK, _TK, _PK, _EK`.

Non-SI units
^^^^^^^^^^

:Classes: :code:`celsius<T>, fahrhenheit<T>`.

:Literals: :code:`_degC, _degF`.

-------------------------------------------------------------------------------

Amount of substance
~~~~~~~~~~~~~~~

SI units
^^^^^^^^^^

:Classes: :code:`attomole<T>, femtomole<T>, picomole<T>, nanomole<T>, micromole<T>, millimole<T>, centimole<T>, decimole<T>, mole<T>, decamole<T>, hectomole<T>, kilomole<T>, megamole<T>, gigamole<T>, teramole<T>, petamole<T>, examole<T>`.

:Literals: :code:`_amol, _fmol, _pmol, _nmol, _umol, _mmol, _mol, _kmol, _Mmol, _Gmol, _Tmol, _Pmol, _Emol`.

-------------------------------------------------------------------------------

Luminous intensity
~~~~~~~~~~~~~~~

SI units
^^^^^^^^^^

:Classes: :code:`attocandela<T>, femtocandela<T>, picocandela<T>, nanocandela<T>, microcandela<T>, millicandela<T>, centicandela<T>, decicandela<T>, candela<T>, decacandela<T>, hectocandela<T>, kilocandela<T>, megacandela<T>, gigacandela<T>, teracandela<T>, petacandela<T>, exacandela<T>`.

:Literals: :code:`_aCd, _fCd, _pCd, _nCd, _uCd, _mCd, _Cd, _kCd, _MCd, _GCd, _TCd, _PCd, _ECd`.

-------------------------------------------------------------------------------

Planar angle
~~~~~~~~~~~~~~~

SI units
^^^^^^^^^^

:Classes: :code:`attoradian<T>, femtoradian<T>, picoradian<T>, nanoradian<T>, microradian<T>, milliradian<T>, centiradian<T>, deciradian<T>, radian<T>, decaradian<T>, hectoradian<T>, kiloradian<T>, megaradian<T>, gigaradian<T>, teraradian<T>, petaradian<T>, exaradian<T>`.

:Literals: :code:`_arad, _frad, _prad, _nrad, _urad, _mrad, _rad, _krad, _Mrad, _Grad, _Trad, _Prad, _Erad`.

Non-SI units
^^^^^^^^^^

:Classes: :code:`degree<T>, turn<T>`.

:Literals: :code:`_deg, _turn`.

-------------------------------------------------------------------------------

Solid angle
~~~~~~~~~~~~~~~

SI units
^^^^^^^^^^

:Classes: :code:`attosteradian<T>, femtosteradian<T>, picosteradian<T>, nanosteradian<T>, microsteradian<T>, millisteradian<T>, centisteradian<T>, decisteradian<T>, steradian<T>, decasteradian<T>, hectosteradian<T>, kilosteradian<T>, megasteradian<T>, gigasteradian<T>, terasteradian<T>, petasteradian<T>, exastearadian<T>`.

:Literals: :code:`_asr, _fsr, _psr, _nsr, _usr, _msr, _sr, _ksr, _Msr, _Gsr, _Tsr, _Psr, _Esr`.

-------------------------------------------------------------------------------

Data/Information quantity
~~~~~~~~~~~~~~~

SI units
^^^^^^^^^^

:Classes: :code:`shannon<T>, bit<T>, nibble<T>, byte<T>, kibibyte<T>, mebibyte<T>, gibibyte<T>, tebibyte<T>, pebibyte<T>`.

:Literals: :code:`_Sh, _b, _B, _kiB, _MiB, _GiB, _TiB, _PiB`.


Derived units
----------------------

-------------------------------------------------------------------------------

Speed
~~~~~~~~~~~~~~~

SI units
^^^^^^^^^^

:Classes: :code:`attometer_per_second<T>, femtometer_per_second<T>, picometer_per_second<T>, nanometer_per_second<T>, micrometer_per_second<T>, millimeter_per_second<T>, centimeter_per_second<T>, decimeter_per_second<T>, meter_per_second<T>, decameter_per_second<T>, hectometer_per_second<T>, kilometer_per_second<T>, megameter_per_second<T>, gigameter_per_second<T>, terameter_per_second<T>, petameter_per_second<T>, exameter_per_second<T>`.

:Literals: :code:`_am_per_s, _fm_per_s, _pm_per_s, _nm_per_s, _um_per_s, _mm_per_s, _m_per_s, _km_per_s, _Mm_per_s, _Gm_per_s, _Tm_per_s, _Pm_per_s, _Em_per_s`.

Non-SI units
^^^^^^^^^^

:Classes: :code:`kilometer_per_hour<T>, knot<T>, mile_per_hour<T>`.

:Literals: :code:`_km_per_h, _kn, _mph`.

-------------------------------------------------------------------------------

Acceleration
~~~~~~~~~~~~~~~

SI units
^^^^^^^^^^

:Classes: :code:`attometer_per_second_squared<T>, femtometer_per_second_squared<T>, picometer_per_second_squared<T>, nanometer_per_second_squared<T>, micrometer_per_second_squared<T>, millimeter_per_second_squared<T>, centimeter_per_second_squared<T>, decimeter_per_second_squared<T>, meter_per_second_squared<T>, decameter_per_second_squared<T>, hectometer_per_second_squared<T>, kilometer_per_second_squared<T>, megameter_per_second_squared<T>, gigameter_per_second_squared<T>, terameter_per_second_squared<T>, petameter_per_second_squared<T>, exameter_per_second_squared<T>`.

:Literals: :code:`_am_per_s2, _fm_per_s2, _pm_per_s2, _nm_per_s2, _um_per_s2, _mm_per_s2, _m_per_s2, _km_per_s2, _Mm_per_s2, _Gm_per_s2, _Tm_per_s2, _Pm_per_s2, _Em_per_s2`.

Non-SI units
^^^^^^^^^^

:Classes: :code:`nanogalileo<T>, microgalileo<T>, milligalileo<T>, galileo<T>`.

:Literals: :code:`_nGal, _uGal, _mGal, _Gal`.

-------------------------------------------------------------------------------

Area
~~~~~~~~~~~~~~~

SI units
^^^^^^^^^^

:Classes: :code:`square_micrometer<T>, square_millimeter<T>, square_centimeter<T>, square_meter<T>, square_kilometer<T>, square_megameter<T>`.

:Literals: :code:`_um2, _mm2, _cm2, _m2, _km2, _Mm2`.

Non-SI units
^^^^^^^^^^

Imperial

:Classes: :code:`square_inch<T>, square_foot<T>, square_yard<T>, square_mile<T>`.

:Literals: :code:`_in2, _ft2, _yd2, _mi2`.

metric

:Classes: :code:`are<T>, hectare<T>`.

:Literals: :code:`_a, _ha`.

-------------------------------------------------------------------------------

Volume
~~~~~~~~~~~~~~~

SI units
^^^^^^^^^^

:Classes: :code:`cubic_millimeter<T>, cubic_centimeter<T>, cubic_meter<T>, cubic_kilometer<T>`.

:Literals: :code:`_mm3, _cm3, _m3, _km3`.

Non-SI units
^^^^^^^^^^

Imperial

:Classes: :code:`cubic_inch<T>, cubic_foot<T>, cubic_yard<T>, cubic_mile<T>`.

:Literals: :code:`_in3, _ft3, _yd3, _mi3`.

metric

:Classes: :code:`microliter<T>, milliliter<T>, liter<T>, hectoliter<T>`.

:Literals: :code:`_uL, _mL, _L, _hL`.

-------------------------------------------------------------------------------

Force
~~~~~~~~~~~~~~~

SI units
^^^^^^^^^^

:Classes: :code:`attonewton<T>, femtonewton<T>, piconewton<T>, nanonewton<T>, micronewton<T>, millinewton<T>, centinewton<T>, decinewton<T>, newton<T>, decanewton<T>, hectonewton<T>, kilonewton<T>, meganewton<T>, giganewton<T>, teranewton<T>, petanewton<T>, exanewton<T>`.

:Literals: :code:`_aN, _fN, _pN, _nN, _uN, _mN, _N, _kN, _MN, _GN, _TN, _PN, _EN`.

Non-SI units
^^^^^^^^^^

:Classes: :code:`pound_force<T>, poundal<T>, kilogram_force<T>, dyne<T>`.

:Literals: :code:`_lbf, _pdl, _kgf, _dyn`.