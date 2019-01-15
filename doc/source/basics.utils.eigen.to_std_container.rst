.. _basics_utils_eigen_to_std_container:

scicpp::utils::eigen::to_std_container
====================================

Defined in header <scicpp/utils.hpp>

Convert an Eigen::Matrix to a standard container.

--------------------------------------

.. function:: template <class EigenMatrix> \
              auto to_std_container(EigenMatrix m)

Return a std::vector if m is a dynamic size matrix,
or a std::array if m is a fixed size matrix.