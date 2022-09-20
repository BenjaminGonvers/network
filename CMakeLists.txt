# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

# Name of the solution
# TODO: Rename solution
project ("network")


include_directories(common/include/)
file(GLOB_RECURSE PROJECT_COMMON common/src/*.cpp common/include/*.h)

# Include sub-projects.
# TODO: Rename project
set(EXECUTABLE_CUSTOM_NAME "network-sfml")
add_subdirectory ("network-sfml")

set(EXECUTABLE_CUSTOM_NAME "NetworkClient")
add_subdirectory ("NetworkClient")


set(EXECUTABLE_CUSTOM_NAME "NetworkServer")
add_subdirectory ("NetworkServer")