find_package (SWIG 3.0.12 REQUIRED)
include (${SWIG_USE_FILE})

# Don't require __declspec's in source for Windows
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Create fir library target
if(WIN32)
    add_library(libfir STATIC fir.hpp fir.cpp)
else()
    add_library(libfir SHARED fir.hpp fir.cpp)
endif()

# Since the shared library target name == libfir, no need for a file prefix
set_target_properties(libfir PROPERTIES PREFIX "")
# Add the directory for target includes
target_include_directories(libfir PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# Tie the project version to the shared library name
set_target_properties(libfir PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(libfir PROPERTIES PUBLIC_HEADER fir.hpp)
# Use c++11
target_compile_features(libfir PUBLIC cxx_std_11)
# Install the shared library target
install(TARGETS libfir
    RUNTIME DESTINATION lib # Help windows out (not used for LINUX)
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include)

# Tag the interface file as CXX
set_source_files_properties (fir.i PROPERTIES CPLUSPLUS ON)

# Function to create python binding
function(_create_binding_python)
    set (Python_ADDITIONAL_VERSIONS 2.7)
    find_package (PythonLibs)
    find_package (PythonInterp)
    if (NOT ${PYTHONLIBS_FOUND} OR NOT ${PYTHONINTERP_FOUND})
        MESSAGE(STATUS "Failed to find Python2.7, skipping python module generation...")
        return ()
    endif()

    swig_add_library (fir TYPE MODULE LANGUAGE python SOURCES fir.i)
    set(_fir ${SWIG_MODULE_fir_REAL_NAME})
    target_include_directories(${_fir} PUBLIC "${PYTHON_INCLUDE_DIR}")
    swig_link_libraries (fir ${PYTHON_LIBRARY} libfir)

    # Find the python site/dist packages directory
    execute_process (COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_lib; print get_python_lib()" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)

    FILE(WRITE ${CMAKE_CURRENT_BINARY_DIR}/__init__.py "")
    # Install the shared library target and python wrapper
    install(TARGETS ${_fir} DESTINATION ${PYTHON_SITE_PACKAGES}/${PROJECT_NAME})
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/__init__.py
        DESTINATION ${PYTHON_SITE_PACKAGES}/${PROJECT_NAME})
    # Some versions of CMake DON'T have the output .py file in a CMake var
    # so do it manually
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/fir.py
        DESTINATION ${PYTHON_SITE_PACKAGES}/${PROJECT_NAME})
endfunction()

# Function to create JAVA binding
function(_create_binding_java)
    find_package (Java)
    find_package (JNI)
    if (NOT ${JAVA_FOUND} OR NOT ${JNI_FOUND})
        MESSAGE(STATUS "Failed to find JDK (with JNI), skipping Java package generation...")
        return ()
    endif()
    # Specify a package name
    list(APPEND CMAKE_SWIG_FLAGS "-package;firjava")
    swig_add_library (firjava TYPE MODULE LANGUAGE java SOURCES fir.i)
    set(_fir ${SWIG_MODULE_firjava_REAL_NAME})
    target_include_directories(${_fir} PUBLIC ${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2} ${JNI_INCLUDE_DIRS})
    swig_link_libraries (${_fir} ${JNI_LIBRARIES} libfir)
endfunction()

# Function to create JS binding
function(_create_binding_js)
    find_package (Node)
    if (NOT ${NODE_FOUND})
        MESSAGE(STATUS "Failed to find NodeJS, skipping JS module generation...")
        return ()
    endif()
    set (V8_VERSION_HEX 0x0${V8_VERSION_MAJOR}${V8_VERSION_MINOR}${V8_VERSION_PATCH})
    string (LENGTH "${V8_VERSION_HEX}" V8_VERSION_HEX_length)
    while (V8_VERSION_HEX_length LESS 8)
        set (V8_VERSION_HEX "${V8_VERSION_HEX}0")
        string (LENGTH "${V8_VERSION_HEX}" V8_VERSION_HEX_length)
    endwhile ()

    # Specify -node flavor and v8 version
    list(APPEND CMAKE_SWIG_FLAGS "-node;-DV8_VERSION=${V8_VERSION_HEX}")
    swig_add_library (firjs TYPE MODULE LANGUAGE javascript SOURCES fir.i)
    set(_fir ${SWIG_MODULE_firjs_REAL_NAME})

    target_include_directories(${_fir} PUBLIC "${NODEJS_INCLUDE_DIRS}")
    swig_link_libraries (${_fir} ${NODE_LIBRARIES} libfir)

    # Simplify the Node module name
    set_target_properties(${_fir} PROPERTIES OUTPUT_NAME fir)

    set_target_properties (${_fir} PROPERTIES
      COMPILE_FLAGS "-DBUILDING_NODE_EXTENSION -DSWIG_V8_VERSION=${V8_VERSION_HEX} "
      PREFIX ""
      SUFFIX ".node")
endfunction()

_create_binding_java()
_create_binding_js()
_create_binding_python()
