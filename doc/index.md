User Manual       {#mainpage}
===========

\tableofcontents

The [fixed_point](http://johnmcfarlane.github.io/fixed_point/) library provides 
a header-only C++11 API for approximating real numbers using binary fixed-point arithmetic.

It forms the work-in-progress reference implementation for a standard library proposal presented in paper, [P0037](papers/p0037.html)
and is developed as part of study groups, [SG14](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14) and SG6.

Installation instructions are provided on the [project page](https://github.com/johnmcfarlane/fixed_point).

Presented here are some examples of what makes this library worth considering for a wide range of applications.

\section declaration Simple

The [fixed_point<>](@ref sg14::fixed_point) type extends the behavior of integer types in a natural way.
It represents a real number using an integer type, scaled by a power of two.

\snippet index.cpp declaration example


\section basic_arithmetic Intuitive

Specializations of [fixed_point<>](@ref sg14::fixed_point) behave a lot like native C/C++ numeric types.
Operators are designed to behave in an way which is both predictable and efficient.

\snippet index.cpp basic arithmetic example


\section advanced_arithmetic Powerful

But one size does not fit all.
Different applications of the same operation might call for different trade-offs between storage, precision, safety and speed.
Named functions - such as [multiply<>](@ref sg14::multiply) - provide fine-tuned control over arithmetic results.

\snippet index.cpp advanced arithmetic example


\section boost Composable

The API is designed with composability in mind
and the `Rep` template parameter is not limited to built-in integer types.
For instance, the `number` types defined in [Boost.Multiprecision](http://www.boost.org/doc/libs/release/libs/multiprecision/)
can be adapted to produce fixed-point types of arbitrary range and resolution.
To illustrate this, a header, [sg14/auxiliary/multiprecision.h](../../include/sg14/auxiliary/multiprecision.h),
is provided, containing all the necessary definitions.

\snippet index.cpp boost example

