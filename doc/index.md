User Manual       {#mainpage}
===========

\tableofcontents


\section Introduction

The [fixed_point](http://johnmcfarlane.github.io/fixed_point/) library provides 
a header-only C++11 API for approximating real numbers using binary fixed-point arithmetic.
It forms the reference implementation for a standard library proposal presented in paper, [P0037](http://wg21.link/p0037)
and is developed as part of study groups, [SG14](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14) and SG6.
Installation instructions are provided on the [project page](https://github.com/johnmcfarlane/fixed_point).


\subsection Features

Header, [sg14/fixed_point](@ref fixed_point), defines class template, [fixed_point](@ref sg14::fixed_point), with:

* a simple, flexible generic design;
* operator overloads that interface [fixed_point](@ref sg14::fixed_point) with other numeric types;
* a set of function templates, (e.g. [multiply](@ref sg14::multiply)), for fine-grain control.

Header, [sg14/num_traits](@ref num_traits), contains additions (such as [set_digits](@ref sg14::set_digits)) 
that support widening of arithmetic types in order to deal with precision loss.

Auxiliary modules:

* [sg14/auxiliary/multiprecision.h](@ref multiprecision.h) adapts Boost.Multiprecision for use in fixed-point types;
* [sg14/auxiliary/elastic_fixed_point.h](@ref elastic_fixed_point.h) defins [elastic_fixed_point](@ref sg14::elastic_fixed_point), an numeric type that uses fixed-point to avoid overflow.


\section Examples

The following examples 
[can be found](https://github.com/johnmcfarlane/fixed_point/blob/master/src/test/index.cpp) 
in the test suite.


\subsection declaration Declaration

The [fixed_point](@ref sg14::fixed_point) type extends the behavior of integer types in a natural way.
It represents a real number using an integer type, scaled by a power of two.

\snippet index.cpp declaration example


\subsection basic_arithmetic Arithmetic Operators

Specializations of [fixed_point](@ref sg14::fixed_point) behave a lot like native C/C++ numeric types.
Operators are designed to behave in an way which is both predictable and efficient.

\snippet index.cpp basic arithmetic example


\subsection advanced_arithmetic Arithmetic Functions

But one size does not fit all.
Different applications of the same operation might call for different trade-offs between storage, precision, safety and speed.
Named functions - such as [multiply](@ref sg14::multiply) - provide fine-tuned control over arithmetic results.

\snippet index.cpp advanced arithmetic example


\subsection elastic Extensible

Because one size does not fit all, [fixed_point](@ref sg14::fixed_point) is designed to make it easy to tailor new arithmetic types. 
The [elastic_fixed_point](@ref sg14::elastic_fixed_point) type illustrates this.
As each calculation requires more digits, so the results of [elastic_fixed_point](@ref sg14::elastic_fixed_point) operations allocate more storage.

\snippet index.cpp elastic example
