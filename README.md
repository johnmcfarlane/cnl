# C++ Binary Fixed-Point Arithmetic

## Introduction

The [fixed_point](https://github.com/johnmcfarlane/fixed_point)
library is designed to represent binary fixed-point real numbers using
built-in integer types. It is developed as part of
[SG14](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14) and
SG6.
For an overview of the API, see draft proposal,
[LEWG, EWG, SG14, SG6: P0037](http://johnmcfarlane.github.io/fixed_point/docs/papers/p0037.html).

## Linux Intructions

Tested on Debian GNU/Linux 8.2.

### Run Tests

    cmake -DCMAKE_BUILD_TYPE=Debug
    make run_tests
    ./run_tests

### Run Benchmarks

    cmake -DCMAKE_BUILD_TYPE=Release
    make run_benchmarks
    ./run_benchmarks

## Windows Instructions

Requires Visual Studio 2015. Tested on Windows 7 Professional.

    C:\Users\john\fixed_point>"c:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe"
vs\fixed_point_test.sln
