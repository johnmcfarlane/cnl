User Manual       {#mainpage}
===========

\tableofcontents

The [fixed_point](http://johnmcfarlane.github.io/fixed_point/) library provides 
a header-only C++11 API for approximating real numbers using binary fixed-point arithmetic.
It forms the reference implementation for a standard library proposal presented in paper, [P0037](papers/p0037.html)
and is developed as part of study groups, [SG14](https://groups.google.com/a/isocpp.org/forum/#!forum/sg14) and SG6.
Installation instructions are provided on the [project page](https://github.com/johnmcfarlane/fixed_point).

The library is a work-in-progress, aiming towards providing general-purpose low-level fixed-point support in the standard.
It include:
* a class template, [fixed_point](@ref sg14::fixed_point), capable of storing any binary fixed-point value:
  * a range of operator overloads that interface [fixed_point](@ref sg14::fixed_point) with built-in types in an intuitive and efficient manner;
  * a set of function templates allowing fine-grain control over the range of arithmetic results, (e.g. [multiply](@ref sg14::multiply));
  * a modern, concept-based design allows for any backend integer type - and through it any rounding style and overflow strategy;
* type traits that support generic widening of arithmetic types in order to avoid precision loss, (e.g. [set_width](@ref sg14::set_width));
* auxiliary modules that:
  * [adapt](../../include/sg14/auxiliary/multiprecision.h) Boost.Multiprecision for use in fixed-point types of arbitrary range and precision and
  * define a numeric type, [elastic](@ref sg14::elastic), which adjusts its capacity to prevent overflow and avoid underflow;
* tests, benchmarks and documentation.

\section declaration Simple

The [fixed_point](@ref sg14::fixed_point) type extends the behavior of integer types in a natural way.
It represents a real number using an integer type, scaled by a power of two.

\snippet index.cpp declaration example


\section basic_arithmetic Intuitive

Specializations of [fixed_point](@ref sg14::fixed_point) behave a lot like native C/C++ numeric types.
Operators are designed to behave in an way which is both predictable and efficient.

\snippet index.cpp basic arithmetic example


\section advanced_arithmetic Powerful

But one size does not fit all.
Different applications of the same operation might call for different trade-offs between storage, precision, safety and speed.
Named functions - such as [multiply](@ref sg14::multiply) - provide fine-tuned control over arithmetic results.

\snippet index.cpp advanced arithmetic example


\section boost Composable

The API is designed with composability in mind
and the `Rep` template parameter is not limited to built-in integer types.
For instance, the `number` types defined in [Boost.Multiprecision](http://www.boost.org/doc/libs/release/libs/multiprecision/)
can be adapted to produce fixed-point types of arbitrary range and resolution.
To illustrate this, a header, [sg14/auxiliary/multiprecision.h](../../include/sg14/auxiliary/multiprecision.h),
is provided, containing all the necessary definitions.

\snippet index.cpp boost example


\section elastic Extensible

Because one size does not fit all, [fixed_point](@ref sg14::fixed_point) is designed to make it easy to tailor new arithmetic types. 
The [elastic](@ref sg14::elastic) type illustrates this.
As each calculation requires more digits, so the results of [elastic](@ref sg14::elastic) operations allocate more storage.

\snippet index.cpp elastic example


\section tests Tested

The library includes an extensive range of compile-time and run-time tests.
Even [the examples on this page](https://github.com/johnmcfarlane/fixed_point/blob/master/src/test/index.cpp) are tested against each new change.

