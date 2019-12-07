# Compositional Numeric Library

[![Build Status](https://travis-ci.org/johnmcfarlane/cnl.svg?branch=master)](https://travis-ci.org/johnmcfarlane/cnl)
[![Build status](https://ci.appveyor.com/api/projects/status/9s7daasmvku9ttvd/branch/master?svg=true)](https://ci.appveyor.com/project/johnmcfarlane/cnl/branch/master)
[![CircleCI](https://circleci.com/gh/johnmcfarlane/cnl/tree/master.svg?style=svg)](https://circleci.com/gh/johnmcfarlane/cnl/tree/master)
[![Join the chat at https://gitter.im/cppcnl/community](https://badges.gitter.im/cppcnl/community.svg)](https://gitter.im/cppcnl/community?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

The Compositional Numeric Library (CNL) is a C++ library of fixed-precision numeric classes which enhance integers to
deliver safer, simpler, cheaper arithmetic types.
Documentation can be found [here](http://johnmcfarlane.github.io/cnl/).

## Requirements

### Linux

Compiled on x86-64 and armv7.
Tested on x86-86 using  [Travis](https://travis-ci.org/johnmcfarlane/cnl) and
[CircleCI](https://circleci.com/gh/johnmcfarlane/cnl).

Requires:

- GCC 5.1 / Clang 3.8

Optional:

- [CMake](https://cmake.org/download/) ([3.5.1](https://docs.travis-ci.com/user/languages/cpp/#CMake))
- [Boost](http://www.boost.org/) - facilitates multiprecision support
- [Conan](https://conan.io/) - package manager makes it easier to
  install test dependencies
- [Doxygen](http://www.doxygen.org/) - generates documentation in the
  *doc/gh-pages* directory

### Windows

Tested on [AppVeyor](https://ci.appveyor.com/project/johnmcfarlane/cnl/branch/master)
and on *Windows 10 Professional* with *CMake 3.8.0*. Requires:

- MSBuild 15.0 (VS 2017)
- [CMake](https://cmake.org/download/) 3.8.0

## Instructions

### Download

The library is [hosted](https://github.com/johnmcfarlane/cnl) on GitHub:

```shell
cd /some/directory
git clone https://github.com/johnmcfarlane/cnl.git
```

### Build

CNL is a header-only library so there is no need to build it.
However, it comes with a number of tests and benchmarks.

#### Running Tests

1. Prepare Conan package manager:

   ```sh
   conan remote add johnmcfarlane/cnl https://api.bintray.com/conan/johnmcfarlane/cnl
   ```

1. Generate the build system:

   ```sh
   mkdir build
   cd build
   cmake -DCMAKE_BUILD_TYPE=Debug -DCNL_DEV=ON ..
   ```

1. Build tests:

   * For Linux:

     ```sh
     cmake --build . --target Tests -- -j $(nproc)
     ```

   * For Windows:

     ```sh
     cmake --build .
     ```

3. Run tests:

   ```sh
   ctest
   ```

#### Running Benchmarks

1. Generate the build system (optimized):

   `cmake -DCMAKE_BUILD_TYPE=Release -DCNL_DEV=ON ..`

2. Build benchmarks:

   `cmake --build . --target Benchmark`

3. Run benchmarks:

   `src/benchmark/Benchmark`

#### Additional build system generation options:

1. To describe CNL build options:

   `cmake -LH ..`

2. Then to apply an option, e.g. to disabled exceptions:

   `cmake -DCNL_EXCEPTIONS=OFF ..`

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
* [BrewBlox firmware](https://github.com/BrewBlox/brewblox-firmware) - firmware for a brewery controller
* [cnl_example](https://github.com/johnmcfarlane/cnl_example) - minimal
  CMake-based project which uses CNL as a dependency.

## Further Reading

- CNL [documentation](http://johnmcfarlane.github.io/cnl/)
- CppCon 2017 [presentation](https://youtu.be/GEfmV3Xcuok)
- Embedded Artistry [article](https://embeddedartistry.com/blog/2017/8/25/c11-fixed-point-arithemetic-library)
- ISO C++ papers:
  - [P0037](http://wg21.link/p0037) - Fixed-Point Real Numbers
  - [P0554](http://wg21.link/p0554) - Composition of Arithmetic Types
  - [P0827](http://wg21.link/p0827) - General-Purpose Constant Value Type
  - [P0828](http://wg21.link/p0828) - Elastic Integers
  - [P1050](http://wg21.link/p1050) - Fractional Numeric Type

## Contact Information

All feedback greatly appreciated.

- [CNL Issues](https://github.com/johnmcfarlane/cnl/issues)
- [SG14 forum](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14)
- [cnl@john.mcfarlane.name](mailto:cnl@john.mcfarlane.name)

Developed with [<img src="doc/logo_CLion.svg" alt="CLion: A Cross-Platform IDE for C and C++ by JetBrains" width="80" />](https://www.jetbrains.com/?from=Crag).
