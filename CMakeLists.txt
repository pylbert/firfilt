cmake_minimum_required (VERSION 3.8.2)
project (firfilt VERSION 1.0.0 LANGUAGES CXX)

# Allow PACKAGENAME_ROOT envars
cmake_policy(SET CMP0074 NEW)

# Add the compiler flags for code coverage on Debug builds
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  include(${PROJECT_SOURCE_DIR}/cmake/modules/CodeCoverage.cmake)
  append_coverage_compiler_flags()
  set(COVERAGE_EXCLUDES '/usr/*' '**tests**' '*build*')
endif()

# Add local CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/modules")

# Find required package for language bindings up-front since their CMake
# variables can be used in all subdirectories

# Required for all language bindings
find_package (SWIG 3.0.12)

# Required for Python module
set (Python_ADDITIONAL_VERSIONS 2.7)
find_package (PythonLibs)
find_package (PythonInterp)

# Required for Java package
find_package (Java)
find_package (JNI)

# Required for JS package
find_package (Node)

# Define the main library TARGET name here as well
set(MAIN_LIB_TARGET_NAME libfir)

# Enable testing prior to adding any subdirectories
enable_testing()

# Shared library (and language bindings)
add_subdirectory(fir)

# Unit tests
add_subdirectory(tests)

# Create a target for test coverage
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  setup_target_for_coverage(
    NAME coverage
    EXECUTABLE ctest -j ${PROCESSOR_COUNT})
endif()
