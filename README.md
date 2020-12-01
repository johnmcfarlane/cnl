# Compositional Numeric Library

[![Test](https://github.com/johnmcfarlane/cnl/workflows/Test/badge.svg)](https://github.com/johnmcfarlane/cnl/actions?query=workflow:Test+branch:main)
[![Join the chat at https://gitter.im/cppcnl/community](https://badges.gitter.im/cppcnl/community.svg)](https://gitter.im/cppcnl/community?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

The Compositional Numeric Library (CNL) is a C++ library of fixed-precision numeric classes which enhance integers to
deliver safer, simpler, cheaper arithmetic types.
Documentation can be found [here](http://johnmcfarlane.github.io/cnl/).

## Requirements

CNL requires a C++11-compatible tool chain and is tested on the following systems:

### Linux

Tested:

- GCC 5 - 10 / Clang 5 - 11
- [CMake](https://cmake.org/download/) 3.5.1

### OS X

Tested:

- GCC 10
- [CMake](https://cmake.org/download/) 3.5.1

### Windows

Tested:

- Visual Studio 2019 Version 16.7
- [CMake](https://cmake.org/download/) 3.8.0

## Instructions

### Download

The library is [hosted](https://github.com/johnmcfarlane/cnl) on GitHub:

```shell
git clone https://github.com/johnmcfarlane/cnl.git
cd cnl
```

### Build

The CNL library is comprised of headers found in the [src](src) directory.
CMake scripts are provided.

To build and install CNL on your system:

```sh
mkdir build && cd build
cmake ..
cmake --build . --target install
```

### Test

The test suite uses CMake and depends on Google Test and Google Benchmark.
Optional integration tests use Boost.Multiprecision and Boost.SIMD.

1. Conan can be used to pull in essential dependencies:

   ```shell
   conan remote add --force johnmcfarlane/cnl https://api.bintray.com/conan/johnmcfarlane/cnl
   conan profile new --detect --force default
   conan profile update settings.compiler.libcxx=libstdc++11 default
   conan install --build=missing ..
   ```

2. Configure the project for development

   ```shell
   cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PROJECT_cnl_INCLUDE:FILEPATH="$(pwd)"/conan_paths.cmake ..
   ```

3. Build tests:

   ```sh
   cmake --build . --target test-all
   ```

4. Run tests:

   ```sh
   ctest -R test-unit
   ```

5. Run benchmarks:

   ```sh
   ctest -R test-benchmark
   ```

### Additional build system generation options:

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
