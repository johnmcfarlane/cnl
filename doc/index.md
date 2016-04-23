User Manual       {#mainpage}
===========

\tableofcontents

\section introduction Introduction

The *fixed_point* library provides 
a header-only C++ API for approximating real numbers using binary fixed-point arithmetic.
It forms the reference implementation of a standard library proposal presented in paper, [P0037](papers/p0037.html)
and is developed as part of study groups, [SG14](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14) and SG6.

Installation instructions are provided on the [project page](https://github.com/johnmcfarlane/fixed_point).

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

