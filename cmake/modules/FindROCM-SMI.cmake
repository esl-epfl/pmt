# - Try to find rocm-smi-lib
# This module tries to find the rocm-smi library on your system
#
# Once done this will define
#  ROCM-SMI_FOUND       - system has rocm-smi-lib
#  ROCM-SMI_INCLUDE_DIR - the rocm-smi-lib include directory
#  ROCM-SMI_LIBRARY     - link these to use rocm-smi-lib

find_package(PackageHandleStandardArgs)

find_library(
  ROCM-SMI_LIBRARY
  NAMES rocm_smi64
  HINTS PATH /opt/rocm
  HINTS ENV LD_LIBRARY_PATH
  PATH_SUFFIXES lib)

find_path(
  ROCM-SMI_INCLUDE_DIR
  NAMES rocm_smi/rocm_smi.h
  HINTS PATH /opt/rocm
  PATH_SUFFIXES include)

find_package_handle_standard_args(ROCM-SMI DEFAULT_MSG ROCM-SMI_LIBRARY
                                  ROCM-SMI_INCLUDE_DIR)
