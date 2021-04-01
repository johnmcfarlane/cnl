# Compositional Numeric Library

[![Test](https://github.com/johnmcfarlane/cnl/workflows/Test/badge.svg)](https://github.com/johnmcfarlane/cnl/actions?query=workflow:Test+branch:main)
[![Join the chat at https://gitter.im/cppcnl/community](https://badges.gitter.im/cppcnl/community.svg)](https://gitter.im/cppcnl/community?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Download](https://api.bintray.com/packages/johnmcfarlane/cnl/cnl%3Ajohnmcfarlane/images/download.svg)](https://bintray.com/johnmcfarlane/cnl/cnl%3Ajohnmcfarlane/_latestVersion)

The Compositional Numeric Library (CNL) is a C++ library of fixed-precision
numeric classes which enhance integers to deliver safer, simpler, cheaper
arithmetic types. Documentation can be found [here](http://johnmcfarlane.github.io/cnl/).

## Requirements

The latest version of CNL requires a C++20-compatible tool chain.
([Version 1.x](https://github.com/johnmcfarlane/cnl/tree/v1.x) supports C++11.)
CNL is tested on the following systems:

### Linux

Tested:

* GCC 5 - 10 / Clang 5 - 11
* [CMake](https://cmake.org/download/) 3.5.1

### OS X

Tested:

* GCC 10
* [CMake](https://cmake.org/download/) 3.5.1

### Windows

Tested:

* Visual Studio 2019 Version 16.7
* [CMake](https://cmake.org/download/) 3.8.0

## Instructions

### Download

The library is [hosted](https://github.com/johnmcfarlane/cnl) on GitHub:

```shell
git clone https://github.com/johnmcfarlane/cnl.git
cd cnl
```

### Build

CMake scripts are provided.

* To build and install CNL on your system:

```sh
mkdir build && cd build
cmake ..
cmake --build . --target install
```

Note: you may need user privileges to install the library.
Alternatively, you can install to user directory using [`CMAKE_INSTALL_PREFIX`](https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX.html#cmake-install-prefix):

```sh
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX:FILE=/home/username/someplace ..
cmake --build . --target install
```

Alternatively, CNL is a header-only library so you can simply point to the
_include_ directory

```sh
c++ -isystem /path/to/cnl/include -std=c++20 my_program.cpp
```

or even include the root header directly in your code:

```c++
#include "/path/to/cnl/include/cnl/all.h"
```

### Test

The test suite uses CMake and depends on Google Test and Google Benchmark.
Optional integration tests use Boost.Multiprecision and Boost.SIMD.

1. Conan can be used to pull in essential dependencies.
   This example assumes GCC but other tool chain files are provided:

   ```shell
   cd build
   conan profile new --detect --force default
   conan profile update settings.compiler.libcxx=libstdc++11 default  # GCC only
   conan profile update settings.compiler.cppstd=20 default
   conan install \
     --build=missing \
     --env CONAN_CMAKE_TOOLCHAIN_FILE="/full/path/to/cnl/test/toolchain/gcc.cmake" \
     --options test=unit \
     ..
   ```

   ... and then configure, build and run unit tests:

   ```shell
   conan build --configure --build --test ..
   ```

1. To run benchmarks, use `--options test=benchmark`...

   ```shell
   conan install --build=missing --options test=benchmark ..
   ```

   then configure and build

   ```shell
   conan build --configure --build ..
   ```

   and finally run explicitly to see the results.

   ```shell
   ./test/benchmark/test-benchmark
   ```

### Integration

The API is exposed through headers in the [include](./include/) directory.
Add this to your system header list and include, e.g.:

```c++
// to use a specific type:
#include <cnl/scaled_integer.h>

// or to include all CNL types:
#include <cnl/all.h>
```

## Example Projects

Examples of projects using CNL:

* [CDSP](https://github.com/hbe72/cdsp) - Compositional DSP Library for
  C++;
* [BrewBlox firmware](https://github.com/BrewBlox/brewblox-firmware) - firmware
  for a brewery controller
* [cnl_example](https://github.com/johnmcfarlane/cnl_example) - minimal
  CMake-based project which uses CNL as a dependency.

## Further Reading

* CNL [documentation](http://johnmcfarlane.github.io/cnl/)
* CppCon 2017 [presentation](https://youtu.be/GEfmV3Xcuok)
* Embedded Artistry [article](https://embeddedartistry.com/blog/2017/8/25/c11-fixed-point-arithemetic-library)
* ISO C++ papers:
  * [P0037](http://wg21.link/p0037) - Fixed-Point Real Numbers
  * [P0554](http://wg21.link/p0554) - Composition of Arithmetic Types
  * [P0827](http://wg21.link/p0827) - General-Purpose Constant Value Type
  * [P0828](http://wg21.link/p0828) - Elastic Integers
  * [P1050](http://wg21.link/p1050) - Fractional Numeric Type

## Contact Information

All feedback greatly appreciated.

* [CNL Issues](https://github.com/johnmcfarlane/cnl/issues)
* [SG14 forum](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14)
* [cnl@john.mcfarlane.name](mailto:cnl@john.mcfarlane.name)
