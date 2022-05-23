#
# Set minimum required version
#
cmake_minimum_required(VERSION 2.8)
project(ARLEQUIN-TESTS)

# Set up ARGoS compilation information
include_directories(${CMAKE_SOURCE_DIR} ${ARGOS_INCLUDE_DIRS})
link_directories(${ARGOS_LIBRARY_DIRS})


add_executable(run_tests tests.cpp)
