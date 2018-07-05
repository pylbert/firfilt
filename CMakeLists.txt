cmake_minimum_required (VERSION 3.8.2)
project (firfilt VERSION 1.0.0 LANGUAGES CXX)

# Add the compiler flags for code coverage on Debug builds
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  include(${PROJECT_SOURCE_DIR}/cmake/modules/CodeCoverage.cmake)
  append_coverage_compiler_flags()
  set(COVERAGE_EXCLUDES '/usr/*' '**tests**' '*build*')
endif()

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
