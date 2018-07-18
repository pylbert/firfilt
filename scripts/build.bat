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

@rem Print out target commands for reference
echo.
echo ================================================================================
echo === Running the following cmake commands =======================================
echo ================================================================================
echo cmake -G %GENERATOR% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -B%ROOT%/build -H%ROOT%
echo cmake --build %ROOT%/build --config %BUILD_TYPE%
echo pushd build
echo ctest -C Release
echo popd
echo ================================================================================
echo.

cmake -G %GENERATOR% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -B%ROOT%/build -H%ROOT%
cmake --build %ROOT%/build --config %BUILD_TYPE%
pushd build
ctest -C Release
popd
