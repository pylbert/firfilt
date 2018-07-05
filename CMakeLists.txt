cmake_minimum_required (VERSION 3.8.2)
project (firfilt
    VERSION 1.0.0
    LANGUAGES CXX)

# Enable testing prior to adding any subdirectories
enable_testing()

# Shared library (and language bindings)
add_subdirectory(fir)

# Unit tests
add_subdirectory(tests)
