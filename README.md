# fixed_point Numerics Library

[![Build Status](https://travis-ci.org/johnmcfarlane/fixed_point.svg?branch=develop)](https://travis-ci.org/johnmcfarlane/fixed_point)
[![Build status](https://ci.appveyor.com/api/projects/status/p60lpkq9u90h83fi/branch/develop?svg=true)](https://ci.appveyor.com/project/johnmcfarlane/fixed-point/branch/develop)

The [fixed_point](https://github.com/johnmcfarlane/fixed_point) library provides 
a header-only C++11 API for approximating real numbers using binary fixed-point arithmetic.
It forms the reference implementation of a standard library proposal presented in paper, [P0037](doc/p0037.md)
and is developed as part of study groups, [SG14](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14) and SG6.

## Download

The library is [hosted](https://github.com/johnmcfarlane/fixed_point) on GitHub:

```shell
$ git clone https://github.com/johnmcfarlane/fixed_point.git
```

The API is exposed through headers in the [include](./include/) directory.
Add this to your system header list and include, e.g.:

```c++
#include <sg14/fixed_point>
```

## Tests and Benchmarks

### Linux

Tested on [Travis](https://travis-ci.org/johnmcfarlane/fixed_point) (Ubuntu 14.04) using GCC 5.4 and Clang 3.5
and [Debian GNU/Linux 8.3](https://www.debian.org/releases/stable/) using GCC 5.4 and Clang 3.5.0. 

Requires:

- G++ 5.4
- [CMake](https://cmake.org/download/) 2.8.11

Optional:

- [Boost](http://www.boost.org/) - facilitates multiprecision support
- [Doxygen](http://www.doxygen.org/) - generates documentation in the *doc/gh-pages* directory
- [cmark](https://github.com/jgm/cmark) - generates proposal papers

For a list of configuration options:

```shell
$ cmake -LH
```

To build everything:

```shell
$ cmake -DCMAKE_BUILD_TYPE=Release
$ make
```

To disable exception handling (incompatible with Boost 1.55 or lower), add `-DEXCEPTIONS=OFF` to the `cmake` command:

```shell
$ cmake -DCMAKE_BUILD_TYPE=Release -DEXCEPTIONS=OFF
$ make
```

To run tests:

```shell
$ cmake -DCMAKE_BUILD_TYPE=Release
$ make fp_test
$ ./fp_test
```

To run benchmarks:

```shell
$ cmake -DCMAKE_BUILD_TYPE=Release
$ make fp_benchmark
$ ./fp_benchmark
```

To profile benchmarks:

1. Build with frame pointers included:

   ```shell
   $ cmake -DCMAKE_BUILD_TYPE=Release -DPROFILE=ON
   $ make fp_benchmark
   $ ./fp_benchmark
   ```

2. then run:

   ```shell
   $ perf record -g ./fp_benchmark
   $ perf report -g 'graph,0.5,caller'
   ```

To install:

```shell
$ cmake -DCMAKE_BUILD_TYPE=Release
$ make
$ sudo make install
```

#### Windows

Tested on [AppVeyor](https://ci.appveyor.com/project/johnmcfarlane/fixed-point/branch/master)
and on *Windows 10 Professional* with *CMake 3.8.0*. Requires:

- MSBuild 15.0 (VS 2017)
- [CMake](https://cmake.org/download/) 3.8.0

To build *vs/Release/fp_test.exe* and *vs/Release/fp_benchmark.exe*:

    cmake -G "Visual Studio 15 2017" .
    MSBuild.exe /m fixed_point.sln /p:Configuration=Release

For 64-bit builds, append `Win64` to the `-G` option above:

    cmake -G "Visual Studio 15 2017 Win64" .

Note that *fp_benchmark* is unlikely to produce valid results due to missing `escape` and `clobber` functions.

### Cleaning

To clean the project files:

    git clean -Xdff .

(Use with caution!)

## Further Reading

- [documentation](http://johnmcfarlane.github.io/fixed_point/)
- [latest draft](doc/p0037.md) of C++ Standard Library proposal, [P0037](doc/p0037.md)
- position paper illustrating the compositonal approach, [P0554](doc/p0554.md)
- forum of [Study Group 14](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14)

## Contact Information

All feedback greatly appreciated.

- [GitHub](https://github.com/johnmcfarlane)
- [SG14 forum](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14)
- [fixed-point@john.mcfarlane.name](mailto:fixed-point@john.mcfarlane.name)
