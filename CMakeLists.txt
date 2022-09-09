# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Name of the solution
# TODO: Rename solution
project ("network")

# Include sub-projects.
# TODO: Rename project
set(EXECUTABLE_CUSTOM_NAME "network-sfml")
add_subdirectory ("network-sfml")
