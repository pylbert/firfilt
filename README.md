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
cmake --build build --target install
```
or
```
sudo make -j24 -Cbuild install
```

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Mike Perkins - Cardinal Peak, LLC
The FIR filter class was designed and written by Mike Perkins.


