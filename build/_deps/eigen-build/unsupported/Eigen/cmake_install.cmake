# Install script for directory: /usr/src/app/build/_deps/eigen-src/unsupported/Eigen

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE FILE FILES
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/AdolcForward"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/AlignedVector3"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/ArpackSupport"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/AutoDiff"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/BVH"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/EulerAngles"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/FFT"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/IterativeSolvers"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/KroneckerProduct"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/LevenbergMarquardt"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/MatrixFunctions"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/MoreVectorization"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/MPRealSupport"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/NonLinearOptimization"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/NumericalDiff"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/OpenGLSupport"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/Polynomials"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/Skyline"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/SparseExtra"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/SpecialFunctions"
    "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/Splines"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE DIRECTORY FILES "/usr/src/app/build/_deps/eigen-src/unsupported/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/usr/src/app/build/_deps/eigen-build/unsupported/Eigen/CXX11/cmake_install.cmake")

endif()

