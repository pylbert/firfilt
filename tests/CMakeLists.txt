# C++ unit tests
add_subdirectory (unit)

if (SWIG_FOUND)
    # Language binding unit tests
    add_subdirectory (java)
    add_subdirectory (js)
    add_subdirectory (python)
endif()
