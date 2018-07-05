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
gtest_add_tests(firfilt_tests "" AUTO)
# Test collateral
configure_file(taps.txt taps.txt.golden COPYONLY)
configure_file(freqresp.txt freqresp.txt.golden COPYONLY)
configure_file(lpf.txt lpf.txt.golden COPYONLY)
configure_file(Fs1000_1Hz_signal_20Hz_noise.txt Fs1000_1Hz_signal_20Hz_noise.txt COPYONLY)
configure_file(filtered_Fs1000_1Hz_signal_20Hz_noise.txt filtered_Fs1000_1Hz_signal_20Hz_noise.txt.golden COPYONLY)

# Add a custom target for unit tests
add_custom_target(tests-unit ALL
    DEPENDS
    firfilt_tests
    COMMENT "FIR filter test collection")
