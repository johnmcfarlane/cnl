# C++ Binary Fixed-Point Arithmetic

[![Build Status](https://travis-ci.org/johnmcfarlane/fixed_point.svg?branch=master)](https://travis-ci.org/johnmcfarlane/fixed_point)
[![Build status](https://ci.appveyor.com/api/projects/status/p60lpkq9u90h83fi/branch/master?svg=true)](https://ci.appveyor.com/project/johnmcfarlane/fixed-point/branch/master)

The [fixed_point](https://github.com/johnmcfarlane/fixed_point) library provides 
a header-only C++11 API for approximating real numbers using binary fixed-point arithmetic.
It forms the reference implementation of a standard library proposal presented in paper, [P0037](papers/p0037.html)
and is developed as part of study groups, [SG14](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14) and SG6.

## Installation

The library is [hosted](https://github.com/johnmcfarlane/fixed_point) on GitHub:

```shell
$ git clone https://github.com/johnmcfarlane/fixed_point.git
```

The API is exposed through headers in the [include](./include/) directory.
Add this to your system header list and include, e.g.:
```c++
#include <sg14/fixed_point.h>
```

## Documentation

Full documentation: [johnmcfarlane.github.io/fixed_point/](http://johnmcfarlane.github.io/fixed_point/)  
C++ standard library proposal, P0037: [johnmcfarlane.github.io/fixed_point/papers/p0037.html](http://johnmcfarlane.github.io/fixed_point/papers/p0037.html)  
A position paper illustrating the elastic numeric type: [johnmcfarlane.github.io/fixed_point/papers/elastic.html](http://johnmcfarlane.github.io/fixed_point/papers/elastic.html)  
The Study Group 14 forum: [groups.google.com/a/isocpp.org/forum/#!forum/sg14](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14)  

## Tests and Benchmarks

### Linux

Tested on [Travis](https://travis-ci.org/johnmcfarlane/fixed_point) (Ubuntu 12.04) using GCC 4.8 and Clang 3.5
and [Debian GNU/Linux 8.3](https://www.debian.org/releases/stable/) using GCC 4.9.2 and Clang 3.5.0. 

Requires:

- GCC 4.8.4
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

To disable exception handling, add `-DEXCEPTIONS=OFF` to the `cmake` command:

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

### Windows

Tested on [AppVeyor](https://ci.appveyor.com/project/johnmcfarlane/fixed-point/branch/master)
and on *Windows 7 Professional* with *CMake 3.4.0*. Requires:

- MSBuild 14.0
- [CMake](https://cmake.org/download/) 2.8.11

To build *vs/Release/fp_test.exe* and *vs/Release/fp_benchmark.exe*:

    cmake .
    MSBuild.exe /m fixed_point.sln /p:Configuration=Release

For 64-bit builds, append `Win64` to the `-G` option above:

    cmake -G "Visual Studio 14 2015 Win64" .

Note that *fp_benchmark* is unlikely to produce valid results due to missing `escape` and `clobber` functions.

### Cleaning

To clean the project files:

    git clean -Xdff .

(Use with caution!)

