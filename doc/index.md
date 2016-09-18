User Manual       {#mainpage}
===========

\tableofcontents


\section Introduction

The [fixed_point](http://johnmcfarlane.github.io/fixed_point/) library provides 
a header-only C++11 API for approximating real numbers using binary fixed-point arithmetic.
It forms the reference implementation for a standard library proposal presented in paper, [P0037](papers/p0037.html)
and is developed as part of study groups, [SG14](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14) and SG6.
Installation instructions are provided on the [project page](https://github.com/johnmcfarlane/fixed_point).


\subsection Features

Header, [sg14/fixed_point.h](@ref fixed_point.h), defines class template, [fixed_point](@ref sg14::fixed_point), with:
  * a simple, flexible generic design;
  * operator overloads that interface [fixed_point](@ref sg14::fixed_point) with other numeric types;
  * a set of function templates, (e.g. [multiply](@ref sg14::multiply)), for fine-grain control.

Header, [sg14/type_traits.h](@ref type_traits.h), contains additions (such as [set_width](@ref sg14::set_width)) 
that support widening of arithmetic types in order to deal with precision loss.

Auxiliary modules:
  * [sg14/auxiliary/multiprecision.h](@ref multiprecision.h) adapts Boost.Multiprecision for use in fixed-point types;
  * [sg14/auxiliary/elastic.h](@ref elastic.h) defins [elastic](@ref sg14::elastic), an numeric type that uses fixed-point to avoid overflow.


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


\subsection boost Composable

The API is designed with composability in mind
and the `Rep` template parameter is not limited to built-in integer types.
For instance, the `number` types defined in [Boost.Multiprecision](http://www.boost.org/doc/libs/release/libs/multiprecision/)
can be adapted to produce fixed-point types of arbitrary range and resolution.
To illustrate this, a header, [sg14/auxiliary/multiprecision.h](../../include/sg14/auxiliary/multiprecision.h),
is provided, containing all the necessary definitions.

\snippet index.cpp boost example

Another supported library is 
[safe_numerics](https://github.com/robertramey/safe_numerics).
It defines an integer type, 
[boost::numeric::safe](https://htmlpreview.github.io/?https://raw.githubusercontent.com/robertramey/safe_numerics/master/doc/html/safe.html), 
which guards against overflow.

\snippet index.cpp safe numerics example


\subsection elastic Extensible

Because one size does not fit all, [fixed_point](@ref sg14::fixed_point) is designed to make it easy to tailor new arithmetic types. 
The [elastic](@ref sg14::elastic) type illustrates this.
As each calculation requires more digits, so the results of [elastic](@ref sg14::elastic) operations allocate more storage.

\snippet index.cpp elastic example
