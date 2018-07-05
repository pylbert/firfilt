find_package (SWIG 3.0.12 REQUIRED)
include (${SWIG_USE_FILE})

# Don't require __declspec's in source for Windows
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Create fir shared library target
add_library(libfir SHARED fir.hpp fir.cpp)
# Since the shared library target name == libfir, no need for a file prefix
set_target_properties(libfir PROPERTIES PREFIX "")
# Add the directory for target includes
target_include_directories(libfir PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# Tie the project version to the shared library name
set_target_properties(libfir PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(libfir PROPERTIES PUBLIC_HEADER fir.hpp)
# Use c++11
target_compile_features(libfir PRIVATE cxx_std_11)
# Install the shared library target
install(TARGETS libfir
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include)

# Tag the interface file as CXX
set_source_files_properties (fir.i PROPERTIES CPLUSPLUS ON)

# Function to create python binding
function(_create_binding_python)
    set (Python_ADDITIONAL_VERSIONS 2.7)
    find_package (PythonLibs REQUIRED)
    find_package (PythonInterp REQUIRED)
    swig_add_library (fir TYPE MODULE LANGUAGE python SOURCES fir.i)
    set(_fir ${SWIG_MODULE_fir_REAL_NAME})
    target_include_directories(${_fir} PUBLIC "${PYTHON_INCLUDE_DIR}")
    swig_link_libraries (fir ${PYTHON_LIBRARY} libfir)

    # Find the python site/dist packages directory
    execute_process (COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_lib; print get_python_lib()" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)

    FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/__init__.py "")
    # Install the shared library target and python wrapper
    install(TARGETS ${_fir} DESTINATION ${PYTHON_SITE_PACKAGES}/${PROJECT_NAME})
    install(FILES ${swig_extra_generated_files} ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
        DESTINATION ${PYTHON_SITE_PACKAGES}/${PROJECT_NAME})
endfunction()

# Function to create JAVA binding
function(_create_binding_java)
    find_package (Java REQUIRED)
    find_package (JNI REQUIRED)
    swig_add_library (fir_java java fir.i)
    target_include_directories(fir_java PUBLIC ${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2} ${JNI_INCLUDE_DIRS})
    swig_link_libraries (fir_java ${JNI_LIBRARIES} libfir)
endfunction()

# Function to create JS binding
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
#    swig_add_library (fir_javascript javascript fir.i)
#else ()
#    swig_add_library (fir_javascript LANGUAGE javascript SOURCES fir.i)
#endif ()
#target_include_directories(fir_javascript PUBLIC "${NODEJS_INCLUDE_DIRS}")
#swig_link_libraries (fir_javascript ${NODE_LIBRARIES} libfir)
#set_target_properties (fir_javascript PROPERTIES
#  COMPILE_FLAGS "-DBUILDING_NODE_EXTENSION -DSWIG_V8_VERSION=${V8_VERSION_HEX} -std=c++11"
#  PREFIX ""
#  SUFFIX ".node"
#)
endfunction()

#_create_binding_java()
#_create_binding_js()
_create_binding_python()
