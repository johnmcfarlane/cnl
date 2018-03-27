# CNL: A Compositional Numeric Library for C++

[![Build Status](https://travis-ci.org/johnmcfarlane/cnl.svg?branch=develop)](https://travis-ci.org/johnmcfarlane/cnl)
[![Build status](https://ci.appveyor.com/api/projects/status/9s7daasmvku9ttvd/branch/develop?svg=true)](https://ci.appveyor.com/project/johnmcfarlane/cnl/branch/develop)

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
and members of [The Kronos Group Inc.](https://www.khronos.org/)
and it serves as a reference implementation for sections of the 
[C++ Numbers Technical Specification](http://wg21.link/p0101).
Please read proposals, [P0037](http://wg21.link/p0037) 
and [P0554](http://wg21.link/p0554) for more details.

## Download

The library is [hosted](https://github.com/johnmcfarlane/cnl) on GitHub:

```shell
$ git clone https://github.com/johnmcfarlane/cnl.git
```

The API is exposed through headers in the [include](./include/) directory.
Add this to your system header list and include, e.g.:

```c++
#include <cnl/fixed_point.h>
```

## Requirements

### Linux

Tested on [Travis](https://travis-ci.org/johnmcfarlane/cnl) (Ubuntu 14.04) 
using GCC 5+ and Clang 3.5+. 

Requires:

- GCC 5.1 / Clang 3.5
- [CMake](https://cmake.org/download/) 3.2

Optional:

- [Boost](http://www.boost.org/) - facilitates multiprecision support
- [Doxygen](http://www.doxygen.org/) - generates documentation in the *doc/gh-pages* directory

### Windows

Tested on [AppVeyor](https://ci.appveyor.com/project/johnmcfarlane/cnl/branch/develop)
and on *Windows 10 Professional* with *CMake 3.8.0*. Requires:

- MSBuild 15.0 (VS 2017)
- [CMake](https://cmake.org/download/) 3.8.0

## Tests and Benchmarks

To run tests:

```shell
$ cmake path/to/project
$ cmake --build .
$ ctest
```

To run benchmarks:

```shell
$ cmake -DCMAKE_BUILD_TYPE=Release
$ cmake --build .
$ src/benchmark/fp_benchmark
```

## Further Reading

- [documentation](http://johnmcfarlane.github.io/cnl/)
- Standard Library fixed-point proposal, [P0037](http://wg21.link/p0037)
- position paper illustrating the compositonal approach, [P0554](http://wg21.link/p0554)
- forum of [Study Group 14](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14)

## Contact Information

All feedback greatly appreciated.

- [GitHub](https://github.com/johnmcfarlane)
- [SG14 forum](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14)
- [cnl@john.mcfarlane.name](mailto:cnl@john.mcfarlane.name)
