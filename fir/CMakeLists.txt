find_package (SWIG 3.0.12 REQUIRED)
include (${SWIG_USE_FILE})

add_library(fir SHARED filt.hpp filt.cpp)
target_include_directories(fir PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_source_files_properties (fir.i PROPERTIES CPLUSPLUS ON)

function(_create_binding_python)
    set (Python_ADDITIONAL_VERSIONS 2.7)
    find_package (PythonLibs REQUIRED)
    find_package (PythonInterp REQUIRED)
    swig_add_module (fir_python python fir.i)
    target_include_directories(_fir_python PUBLIC "${PYTHON_INCLUDE_DIR}")
    swig_link_libraries (fir_python ${PYTHON_LIBRARY} fir)
endfunction()

function(_create_binding_java)
    # JAVA
    find_package (Java REQUIRED)
    find_package (JNI REQUIRED)
    swig_add_module (fir_java java fir.i)
    target_include_directories(fir_java PUBLIC ${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2} ${JNI_INCLUDE_DIRS})
    swig_link_libraries (fir_java ${JNI_LIBRARIES} fir)
endfunction()

function(_create_binding_js)
#find_package (Node REQUIRED)
#set (V8_VERSION_HEX 0x0${V8_VERSION_MAJOR}${V8_VERSION_MINOR}${V8_VERSION_PATCH})
#string (LENGTH "${V8_VERSION_HEX}" V8_VERSION_HEX_length)
#while (V8_VERSION_HEX_length LESS 8)
#    set (V8_VERSION_HEX "${V8_VERSION_HEX}0")
#    string (LENGTH "${V8_VERSION_HEX}" V8_VERSION_HEX_length)
#endwhile ()
#
#set_property(SOURCE fir.i PROPERTY SWIG_FLAGS "-node" "-DV8_VERSION=${V8_VERSION_HEX}")
#if (CMAKE_VERSION VERSION_LESS "3.8")
#    swig_add_module (fir_javascript javascript fir.i)
#else ()
#    swig_add_library (fir_javascript LANGUAGE javascript SOURCES fir.i)
#endif ()
#target_include_directories(fir_javascript PUBLIC "${NODEJS_INCLUDE_DIRS}")
#swig_link_libraries (fir_javascript ${NODE_LIBRARIES} fir)
#set_target_properties (fir_javascript PROPERTIES
#  COMPILE_FLAGS "-DBUILDING_NODE_EXTENSION -DSWIG_V8_VERSION=${V8_VERSION_HEX} -std=c++11"
#  PREFIX ""
#  SUFFIX ".node"
#)
endfunction()

_create_binding_java()
_create_binding_js()
_create_binding_python()
