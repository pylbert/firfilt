# Don't require __declspec's in source for Windows
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Create fir library target
if(WIN32)
    add_library(${MAIN_LIB_TARGET_NAME} STATIC fir.hpp fir.cpp)
else()
    add_library(${MAIN_LIB_TARGET_NAME} SHARED fir.hpp fir.cpp)
endif()

# Since the shared library target name == libfir, no need for a file prefix
set_target_properties(${MAIN_LIB_TARGET_NAME} PROPERTIES PREFIX "")
# Add the directory for target includes
target_include_directories(${MAIN_LIB_TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# Tie the project version to the shared library name
set_target_properties(${MAIN_LIB_TARGET_NAME} PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(${MAIN_LIB_TARGET_NAME} PROPERTIES PUBLIC_HEADER fir.hpp)
# Use c++11
target_compile_features(${MAIN_LIB_TARGET_NAME} PUBLIC cxx_std_11)
# Install the shared library target
install(TARGETS ${MAIN_LIB_TARGET_NAME}
    RUNTIME DESTINATION lib # Help windows out (not used for LINUX)
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include)

# Tag the interface file as CXX
set_source_files_properties (fir.i PROPERTIES CPLUSPLUS ON)

# Function to create python binding
function(_create_binding_python)
    if (NOT ${PYTHONLIBS_FOUND} OR NOT ${PYTHONINTERP_FOUND})
        MESSAGE(STATUS "Failed to find Python2.7, skipping python module generation...")
        return ()
    endif()

    swig_add_library (fir TYPE MODULE LANGUAGE python SOURCES fir.i)
    set(_fir ${SWIG_MODULE_fir_REAL_NAME})
    target_include_directories(${_fir} PUBLIC "${PYTHON_INCLUDE_DIR}")
    swig_link_libraries (fir ${PYTHON_LIBRARY} ${MAIN_LIB_TARGET_NAME})

    # Find the python site/dist packages directory
    execute_process (COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_lib; print get_python_lib()" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)

    # MSVS generators place the python binary in Release/Debug but the ctest
    # needs it local to fir.py
    if (MSVC)
        add_custom_command(TARGET ${_fir} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:${_fir}>"
            "${CMAKE_CURRENT_BINARY_DIR}"
            COMMENT "Copy python binary to ${CMAKE_CURRENT_BINARY_DIR} for ctests")
    endif()
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
    if (NOT ${JAVA_FOUND} OR NOT ${JNI_FOUND})
        MESSAGE(STATUS "Failed to find JDK (with JNI), skipping Java package generation...")
        return ()
    endif()
    # Specify a package name
    list(APPEND CMAKE_SWIG_FLAGS "-package;firjava")
    swig_add_library (firjava TYPE MODULE LANGUAGE java SOURCES fir.i)
    set(_fir ${SWIG_MODULE_firjava_REAL_NAME})
    target_include_directories(${_fir} PUBLIC ${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2} ${JNI_INCLUDE_DIRS})
    swig_link_libraries (${_fir} ${JNI_LIBRARIES} ${MAIN_LIB_TARGET_NAME})

    # Compile/zip the java package
    add_custom_command (TARGET ${_fir}
          POST_BUILD
          COMMAND cmake -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/firjava
          COMMAND ${JAVA_COMPILE} *.java -d ${CMAKE_CURRENT_BINARY_DIR} -cp .
          COMMAND ${JAVA_ARCHIVE} cf firjava.jar firjava
          COMMENT "Build firjava package")

    install (FILES ${CMAKE_CURRENT_BINARY_DIR}/firjava.jar
        DESTINATION lib/java
        COMPONENT ${CMAKE_PROJECT_NAME}-java)

    install(TARGETS ${_fir}
        RUNTIME DESTINATION lib
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
endfunction()

# Function to create JS binding
function(_create_binding_js)
    if (NOT ${NODE_FOUND})
        MESSAGE(STATUS "Failed to find Node, skipping JS module generation...")
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
    swig_link_libraries (${_fir} ${NODE_LIBRARIES} ${MAIN_LIB_TARGET_NAME})

    # Simplify the Node module name
    set_target_properties(${_fir} PROPERTIES OUTPUT_NAME fir)

    set_target_properties (${_fir} PROPERTIES
      COMPILE_FLAGS "-DBUILDING_NODE_EXTENSION -DSWIG_V8_VERSION=${V8_VERSION_HEX} "
      PREFIX ""
      SUFFIX ".node")
endfunction()

# If no SWIG, don't attempt the language bindings
if (SWIG_FOUND)
    include (${SWIG_USE_FILE})
else ()
    message(STATUS "Install SWIG to enable language bindings")
    return ()
endif()

_create_binding_java()
_create_binding_js()
_create_binding_python()
