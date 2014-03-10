# - Config file for the OctoMap package
# (example from http://www.vtk.org/Wiki/CMake/Tutorials/How_to_create_a_ProjectConfig.cmake_file)
# It defines the following variables
#  OCTOMAP_INCLUDE_DIRS - include directories for OctoMap
#  OCTOMAP_LIBRARY_DIRS - library directories for OctoMap (normally not used!)
#  OCTOMAP_LIBRARIES    - libraries to link against
 
# Tell the user project where to find our headers and libraries
set(DYNAMICEDT3D_INCLUDE_DIRS "/usr/local/include")
set(DYNAMICEDT3D_LIBRARY_DIRS "/usr/local/lib")
 
# Our library dependencies (contains definitions for IMPORTED targets)
# include("/FooBarLibraryDepends.cmake")
 
set(DYNAMICEDT3D_LIBRARIES dynamicedt3d)
