User Manual       {#mainpage}
===========

\tableofcontents

\section introduction Introduction

The [fixed_point](https://github.com/johnmcfarlane/fixed_point) library provides 
a header-only C++ API for approximating real numbers using binary fixed-point arithmetic.
It forms the reference implementation of a standard library proposal presented in paper, [P0037](papers/p0037.html)
and is developed as part of study groups, [SG14](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14) and SG6.


\section declaration Declaring a Value

The [fixed_point<Rep, Exponent>](@ref sg14::fixed_point) class template
represents a real number using an integer type, scaled by a power of two.
 
\snippet index.cpp declaration example


\section basic_arithmetic Basic Arithmetic

Specializations of [fixed_point<>](@ref sg14::fixed_point) behave a lot like native C/C++ numeric types.

\snippet index.cpp basic arithmetic example


\section advanced_arithmetic Advanced Arithmetic

Unlike floating-point types, fixed-point numbers overflow easily.
Named functions are provided for fine-tuned control, e.g. [multiply<>](@ref sg14::multiply).

\snippet index.cpp advanced arithmetic example


\section boost Boost.Multiprecision Example

The API is designed with extensibility in mind
and the `Rep` template parameter is not limited to built-in integer types.
For instance, the `number` types defined in [Boost.Multiprecision](http://www.boost.org/doc/libs/release/libs/multiprecision/)
can be used to produce fixed-point types of arbitrary range and resolution.

To illustrate this, a header, [sg14/auxiliary/multiprecision.h](../../include/sg14/auxiliary/multiprecision.h),
contains all the definitions necessary to adapt 
[Boost.Multiprecision](http://www.boost.org/doc/libs/release/libs/multiprecision/) 
for use with [fixed_point](@ref sg14::fixed_point).

\snippet index.cpp boost example


\section installation Installation

The library is [hosted](https://github.com/johnmcfarlane/fixed_point) on GitHub:

    git clone https://github.com/johnmcfarlane/fixed_point.git

\section tests_and_benchmarks Tests and Benchmarks

\subsection linux_and_unix Linux / UNIX

Tested on [Travis](https://travis-ci.org/johnmcfarlane/fixed_point) (Ubuntu 12.04) using GCC 4.8 and Clang 3.5
and [Debian GNU/Linux 8.2](https://www.debian.org/releases/stable/) using GCC 4.9.2 and Clang 3.5.0. 

To build from the project directory:

    cmake -DCMAKE_BUILD_TYPE=Release
    make

To disable exception handling, add `-DEXCEPTIONS=OFF` to the `cmake` command:

    cmake -DCMAKE_BUILD_TYPE=Release -DEXCEPTIONS=OFF
    make

To run tests:

    ./fp_tests

To run benchmarks:

    ./fp_benchmarks

To profile benchmarks:

1. in *src/benchmark/CMakeLists.txt*, append `-fno-omit-frame-pointer` to `COMPILE_FLAGS`:

       PROPERTIES COMPILE_FLAGS "${COMMON_CXX_FLAGS} -fno-omit-frame-pointer"

2. then run:

       perf record -g ./fp_benchmark
       perf report -g 'graph,0.5,caller'

\subsection windows Windows

Tested on [AppVeyor](https://ci.appveyor.com/project/johnmcfarlane/fixed-point/branch/master)
and on *Windows 7 Professional* with *CMake 3.4.0*. Requires:

- MSBuild 14.0
- [CMake](https://cmake.org/download/) 2.8.4

To build *vs/Release/fp_test.exe* and *vs/Release/fp_benchmark.exe*:

    cmake .
    MSBuild.exe /m fixed_point.sln /p:Configuration=Release

For 64-bit builds, append `Win64` to the `-G` option above:

    cmake -G "Visual Studio 14 2015 Win64" .

Note that *fp_benchmark* is unlikely to produce valid results due to missing `escape` and `clobber` functions.

\subsection cleaning Cleaning

To clean the project files:

    git clean -Xdff .

(Use with caution!)


\section further Further Reading

For an overview of the API, see proposal,
[P0037](http://johnmcfarlane.github.io/fixed_point/papers/p0037.html).

For an example of how to use the API to make a general-purpose fixed-point type, see 
[this paper](http://johnmcfarlane.github.io/fixed_point/papers/elastic.html).

It is developed as part of
[SG14](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14) and SG6.
