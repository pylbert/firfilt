# firfilt

The firfilt project is a cross-platform, light-weight C++ FIR implementation.  The intent is to provide a simplified example of using the CMake build system with SWIG for generating Java, JS, and Python language extensions.

## Getting Started

### Prerequisites

CMake [and optionally SWIG, a JDK, NodeJS/V8, and Python] 

### Clone/Build/Install

#### LINUX Makefiles
```
git clone https://github.com/pylbert/firfilt.git
cmake -Bbuild -H.
make -j24 -Cbuild install DESTDIR=/tmp/blah
```
or
```
make -j24 -Cbuild
sudo make -j24 -Cbuild install
```

#### Windows VS Nmake Makefiles
```
git clone https://github.com/pylbert/firfilt.git
cmake -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Release -Bbuild -H.
# Note, may require a command shell with admin privileges
cmake --build build --target install
```

## Googletest Unit Testing

A collection of unit tests are included which test the filter methods.

Building/running the unit tests

```
make -j24 -Cbuild test
```
or
```
cd build
make test
```
or
```
cd build
ctest
```

## Code Coverage

The unit tests also provide coverage via gcov/lcov (if installed).

Generating coverage

```
make -j24 -Cbuild coverage
```

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Mike Perkins - Cardinal Peak, LLC
The FIR filter class was designed and written by Mike Perkins.  The CMake project, accompanying files, and some C++ touch-up was done by Noel Eck.


