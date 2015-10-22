# C++ Binary Fixed-Point Arithmetic

## Introduction

The [fixed_point](https://github.com/johnmcfarlane/fixed_point)
library is designed to represent binary fixed-point real numbers using
built-in integer types. It is developed as part of
[SG14](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14) and
SG6.
For an overview of the API, see draft proposal,
[LEWG, EWG, SG14, SG6: P0037](http://johnmcfarlane.github.io/fixed_point/docs/papers/p0037.html).

## Intructions

### Get

    git clone https://github.com/johnmcfarlane/fixed_point.git --recursive

### Run Tests

    cmake -DCMAKE_BUILD_TYPE=Debug -DRUN_TESTS=ON -DRUN_BENCHMARKS=OFF
    make
    ./fixed_point_test

### Run Benchmarks

    cmake -DCMAKE_BUILD_TYPE=Release -DRUN_TESTS=OFF -DRUN_BENCHMARKS=ON
    make
    ./fixed_point_benchmark
