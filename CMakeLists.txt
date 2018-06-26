cmake_minimum_required (VERSION 2.8.11)
project (swig_cmake_basics)

#set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

find_package (PkgConfig REQUIRED)

## Cmake print all variables
#get_cmake_property(_variableNames VARIABLES)
#foreach (_variableName ${_variableNames})
#    message(STATUS "XXX ${_variableName}=${${_variableName}}")
#endforeach()
#message(FATAL_ERROR "done")


add_subdirectory(fir)
