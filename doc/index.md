User Manual       {#mainpage}
===========

\tableofcontents


\section Introduction

The [CNL](http://johnmcfarlane.github.io/cnl/) library provides 
a header-only C++11/14/17 API for extending the usefulness of integers.
It forms the reference implementation for a standard library proposal presented in paper, [P0037](http://wg21.link/p0037)
and is developed as part of study groups, [SG14](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14) and SG6.
Installation instructions are provided on the [project page](https://github.com/johnmcfarlane/cnl).


\subsection Features

Header, [cnl/fixed_point.h](@ref cnl::fixed_point), defines class template, [fixed_point](@ref cnl::fixed_point), with:

* a simple, flexible generic design;
* operator overloads that interface [fixed_point](@ref cnl::fixed_point) with other numeric types;
* a set of function templates, (e.g. [multiply](@ref cnl::multiply)), for fine-grain control.

Header, [cnl/num_traits.h](@ref cnl/num_traits.h), contains additions (such as [set_digits](@ref cnl::set_digits)) 
that support widening of arithmetic types in order to deal with precision loss.

Auxiliary modules:

* [cnl/auxiliary/boost.multiprecision.h](@ref boost.multiprecision.h) adapts Boost.Multiprecision for use in fixed-point types;
* [cnl/auxiliary/boost.simd.h](@ref boost.simd.h) adapts Boost.SIMD for use in fixed-point types;


\section Examples

The following examples 
[can be found](https://github.com/johnmcfarlane/cnl/blob/master/src/test/index.cpp) 
in the test suite.


\subsection declaration Declaration

The [fixed_point](@ref cnl::fixed_point) type extends the behavior of integer types in a natural way.
It represents a real number using an integer type, scaled by a power of two.

\snippet index.cpp declaration example


\subsection basic_arithmetic Arithmetic Operators

Specializations of [fixed_point](@ref cnl::fixed_point) behave a lot like native C/C++ numeric types.
Operators are designed to behave in an way which is both predictable and efficient.

\snippet index.cpp basic arithmetic example


\subsection advanced_arithmetic Arithmetic Functions

But one size does not fit all.
Different applications of the same operation might call for different trade-offs between storage, precision, safety and speed.
Named functions - such as [multiply](@ref cnl::multiply) - provide fine-tuned control over arithmetic results.

\snippet index.cpp advanced arithmetic example


\subsection elastic Extensible

Because one size does not fit all, [fixed_point](@ref cnl::fixed_point) is designed to make it easy to tailor new arithmetic types. 
The [elastic_number](@ref cnl::elastic_number) type illustrates this.
As each calculation requires more digits, so the results of [elastic_number](@ref cnl::elastic_number) operations allocate more storage.

\snippet index.cpp elastic example
