# CNL: A Compositional Numeric Library for C++

[![Build Status](https://travis-ci.org/johnmcfarlane/cnl.svg?branch=develop)](https://travis-ci.org/johnmcfarlane/cnl)
[![Build status](https://ci.appveyor.com/api/projects/status/9s7daasmvku9ttvd/branch/develop?svg=true)](https://ci.appveyor.com/project/johnmcfarlane/cnl/branch/develop)
[![CircleCI](https://circleci.com/gh/johnmcfarlane/cnl/tree/develop.svg?style=svg)](https://circleci.com/gh/johnmcfarlane/cnl/tree/develop)

[CNL](https://github.com/johnmcfarlane/cnl) is a numerics library written in C++
which aims to do for `int` what the STL does for arrays.
Its main roles are to help:

* approximate real numbers using fixed-point arithmetic;
* detect and prevent overflow and
* increase precision through alternative rounding modes.

A compositional design promotes seamless interoperability between numeric types.
And by providing the thinnest wrappers over the widest range of numeric types,
CNL aims to assist the development of:

* large-scale simulations including video games and scientific models;
* resource-constrained applications on embedded and mobile devices and
* math-intensive operations on specialized hardware such as GPUs and FPGAs.

The library's APIs are developed in colaboration with standardization
bodies such as [The C++ Standards Committee](http://www.open-std.org/jtc1/sc22/wg21/)
and members of [The Khronos Group Inc.](https://www.khronos.org/)
and it serves as a reference implementation for sections of the
[C++ Numbers Technical Specification](http://wg21.link/p0101).
Please read proposals, [P0037](http://wg21.link/p0037)
and [P0554](http://wg21.link/p0554) for more details.

## Requirements

### Linux

Tested on [Travis](https://travis-ci.org/johnmcfarlane/cnl) and [CircleCI](https://circleci.com/gh/johnmcfarlane/cnl).

Requires:

- GCC 5.1 / Clang 3.5
- [CMake](https://cmake.org/download/) ([3.0.2](https://docs.travis-ci.com/user/languages/cpp/#CMake))

Optional:

- [Boost](http://www.boost.org/) - facilitates multiprecision support
- [Doxygen](http://www.doxygen.org/) - generates documentation in the *doc/gh-pages* directory

### Windows

Tested on [AppVeyor](https://ci.appveyor.com/project/johnmcfarlane/cnl/branch/develop)
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

1. Generate the build system:

   `cmake /some/directory/cnl`

2. Build tests:

   * For Linux (in parallel using *N* cores):

     `cmake --build . --target Tests -- -j N`

   * For Windows:

     `cmake --build . --target src/test/Tests`

3. Run tests:

   `ctest`

#### Running Benchmarks

1. Generate the build system (optimized):

   `cmake -DCMAKE_BUILD_TYPE=Release /some/directory/cnl`

2. Build benchmarks:

   `cmake --build . --target Benchmark`

3. Run benchmarks:

   `src/benchmark/Benchmark`

#### Additional build system generation options:

1. To describe CNL build options:

   `cmake -LH /some/directory/cnl`

2. Then to apply an option, e.g. to disabled exceptions:

   `cmake -DEXCEPTIONS=OFF /some/directory/cnl`

### Integration

The API is exposed through headers in the [include](./include/) directory.
Add this to your system header list and include, e.g.:

```c++
// to use a specific type:
#include <cnl/fixed_point.h>

// or to include all CNL types:
#include <cnl/all.h>
```

See example program, [Tau](./example/tau/README.md), for more details.

## Further Reading

- [documentation](http://johnmcfarlane.github.io/cnl/)
- Standard Library fixed-point proposal, [P0037](http://wg21.link/p0037)
- position paper illustrating the compositonal approach, [P0554](http://wg21.link/p0554)
- forum of [Study Group 14](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14)

## Contact Information

All feedback greatly appreciated.

- [CNL Issues](https://github.com/johnmcfarlane/cnl/issues)
- [SG14 forum](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14)
- [cnl@john.mcfarlane.name](mailto:cnl@john.mcfarlane.name)

Developed with [<img src="doc/logo_CLion.svg" alt="CLion: A Cross-Platform IDE for C and C++ by JetBrains" width="63" height="14" />](https://www.jetbrains.com/clion/).
