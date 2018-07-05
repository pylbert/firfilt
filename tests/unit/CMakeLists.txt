# If gtest is NOT found, don't build the test suite
find_package(GTest)

# If not found, print a status message and return
if(NOT GTEST_FOUND)
    message(STATUS "Install Google Test to enable additional unit testing")
    return ()
endif()

# Unit tests
add_executable(firfilt_tests firfilt_tests.cpp)
target_link_libraries(firfilt_tests libfir GTest::GTest GTest::Main)

# Helper header file for test collateral
configure_file(firfilt_tests.hpp.in firfilt_tests.hpp)

target_include_directories(firfilt_tests PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

gtest_add_tests(firfilt_tests "" AUTO)

# Add a custom target for unit tests
add_custom_target(tests-unit ALL
    DEPENDS
    firfilt_tests
    COMMENT "FIR filter test collection")
