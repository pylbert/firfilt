set(CTEST_OUTPUT_ON_FAILURE 1)

# C++ unit tests
add_subdirectory (unit)

# Language binding unit tests
add_subdirectory (java)
add_subdirectory (js)
add_subdirectory (python)
