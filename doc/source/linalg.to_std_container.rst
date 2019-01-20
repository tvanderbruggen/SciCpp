.. _linalg_to_std_container:

scicpp::linalg::to_std_container
====================================

Defined in header <scicpp/linalg.hpp>

Convert an Eigen::Matrix to a standard container.

--------------------------------------

.. function:: template <class EigenMatrix> \
              auto to_std_container(EigenMatrix m)

Return a std::vector if m is a dynamic size matrix,
or a std::array if m is a fixed size matrix.