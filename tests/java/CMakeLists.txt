if (NOT ${JAVA_FOUND} OR NOT ${JNI_FOUND})
    return()
endif()

include(UseJava)

get_property(MAIN_LIB_BUILD_DIR TARGET ${MAIN_LIB_TARGET_NAME} PROPERTY BINARY_DIR)

add_jar(Test SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Test.java
    INCLUDE_JARS "${MAIN_LIB_BUILD_DIR}/firjava.jar"
    OUTPUT_NAME Test)
add_dependencies(Test firjava)

add_test (NAME test_java
    COMMAND ${JAVA_RUNTIME} -Djava.library.path=${MAIN_LIB_BUILD_DIR}
    -cp ${MAIN_LIB_BUILD_DIR}/firjava.jar:Test.jar Test
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
