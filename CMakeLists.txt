cmake_minimum_required (VERSION 3.8.2)
project (firfilt
    VERSION 1.0.0
    LANGUAGES CXX)

#find_package (PkgConfig REQUIRED)

## Cmake print all variables
#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "XXX ${_variableName}=${${_variableName}}")
#endforeach()
#message(FATAL_ERROR "done")


add_subdirectory(fir)
