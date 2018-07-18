@echo off

@rem Pick a generator (default to NMake)
set GENERATOR="Visual Studio 14"
set GENERATOR="Eclipse CDT4 - NMake Makefiles"
set GENERATOR="NMake Makefiles"
set BUILD_TYPE=Release
set SWIG_ROOT=C:/swigwin-3.0.12
set GTEST_ROOT=C:/googletest-distribution
set NODE_ROOT=
set JAVA_HOME=

@rem Use git to determine top-level directory
FOR /F "tokens=* USEBACKQ" %%F IN (`git rev-parse --show-toplevel`) DO (SET ROOT=%%F)

set cmd_gen=cmake -G %GENERATOR% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -B%ROOT%/build -H%ROOT%
set cmd_build=cmake --build %ROOT%/build --config %BUILD_TYPE%
set cmd_test=ctest -C Release

@rem Print out target commands for reference
echo.
echo ================================================================================
echo === Running the following cmake commands =======================================
echo ================================================================================
echo %cmd_gen%
echo %cmd_build%
echo pushd build
echo %cmd_test%
echo popd
echo ================================================================================
echo.

%cmd_gen%
%cmd_build%
pushd build
%cmd_test%
popd
