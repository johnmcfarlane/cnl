**Document number**: LEWG, SG14, SG6: DXXXX  
**Date**: 2016-03-27  
**Reply-to**: John McFarlane, [fixed-point@john.mcfarlane.name](mailto:fixed-point@john.mcfarlane.name)

# Elastic Number Type

## Introduction

This paper addresses feedback provided in response to the proposal in 
[P0037](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0037r1.html). 
Particular attention is paid to comparisons 
between [P0037](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0037r1.html) 
and the similar proposal in [P0106](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0106r0.html).

## Background

### Existing Papers

In [P0037](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0037r1.html), 
the author introduces a binary fixed-point arithmetic type, `fixed_point`, 
which approximates real numbers by storing an integer value that is shifted up or down by a fixed number of bits. 
It automates laborious, error-prone operations which are routinely performed in situations where floating-point cannot be used. 
It resembles the APIs provided by existing libraries such as fp and fixed-point.

Lawrence Crowl's paper, [P0106](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0106r0.html), 
details various numeric types including fixed-point types, `nonnegative` and `negatable`. 
These types are intended to cater for a wide variety of users 
by offering customizable strategies for error handling and precision loss.

### Feedback on P0037

Valuable feedback on [P0037](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0037r1.html) 
and [P0106](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0106r0.html) 
has been provided 
by - among others - SG6 at [Kona in 2015](https://groups.google.com/a/isocpp.org/forum/?utm_medium=email&utm_source=footer#!msg/sg14/-Dw-jYbhlAc/E_ifUGPMDgAJ) 
and [Jacksonville in 2016](https://groups.google.com/a/isocpp.org/forum/?utm_medium=email&utm_source=footer#!msg/sg14/ak-1E9-0PsE/h6Bs_CSCAQAJ) 
as well as SG14 at [CppCon 2015](https://groups.google.com/a/isocpp.org/forum/#!searchin/sg14/minutes/sg14/ey5o_ZIxSXs/aTLtbKdWBQAJ) 
and [GDC 2016](https://groups.google.com/a/isocpp.org/d/msg/sg14/Euroc46c-n8/4oiHpt5fAAAJ). 

Among the observations of the `fixed_point` type in [P0037](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0037r1.html),
some reservations were:
* it does not handle overflow;
* it lacks many desired rounding modes;
* its capacity is limited by the capacity of the integer value used as its representational type;
* it possesses a most negative number whose magnitude is greater than its most positive value and
* it is limited to base two numbers.

A general recommendation was that the proposals 
in [P0037](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0037r1.html) 
and [P0106](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0106r0.html) 
should be combined into a single effort.

## Discussion

Most of the feedback items (including those listed above) are aimed at `fixed_point` as compared to 
the numeric types detailed in [P0106](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0106r0.html). 
However, it is not the intent of `fixed_point` to address all of the same concerns. 
Its aim is to do one job: to approximate real numbers using fixed-point arithmetic.
In doing so, it can support a very wide range of fixed-point styles,
from traditional embedded systems applications involving fixed-size types
to the adaptively-sized types described in
[P0106](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0106r0.html).

The following section introduces a set of arithmetic class templates, of which `fixed_point` is one.
Each class concentrates on solving a single problem with a self-contained solution.
They combine to produce a type which tackles all of the major issues addressed in
[P0106](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0106r0.html).

## Example of Composable Numeric Types

### The `custom_integer` Class Template

The `custom_integer` type provides template parameters for 
fine-tuning rounding style, overflow handling and capacity - 
much in the spirit of the `cardinal` and `integral` types proposed in 
[P0106](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0106r0.html).
For ease of comparison, it adopts the same `rounding` and `overflow` enumerations defined in 
Lawrence Crowl's paper, [P0105](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0105r0.html)
but adds `native` members which specify the behavior of built-in arithmetic types.

``` c++
template <
    rounding Crnd = rounding::native, 
    overflow Covf = overflow::native, 
    int NumDigits = numeric_limits<int>::digits(), 
    bool IsSigned = true>
class custom_integer;
```

When default template parameters are chosen, `custom_integer` behaves as closely as possible to `int`. 
More generally, it is intended that `custom_integer` can provide 
a drop-in replacement for `int` in a variety of applications. 

The range of `custom_integer` may exceed `NumDigits` in order to exploit machine registers. 
For example, when `NumDigits==1`, it is inevitable that the type will be at least a byte in size. 

It is likely that `custom_integer` can further be decomposed 
into types which implement custom sizes, rounding modes and overflow strategies. 
Details are beyond the scope of this paper but to illustrate this composition, 
the following alternative definition of `custom_integer` is given:

``` c++
template <round Crnd, overflow Covf, int NumDigits, bool IsSigned>
using custom_integer = overflow_integer<Covf, rounding_integer<Crnd, sized_integer<NumDigits, IsSigned>>>;
```

### The `fixed_point` Class Template

This class template is described in detail in [P0037](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0037r1.html).
It stores an integer of type, `ReprType`, which is left-shifted by fixed amount, `Exponent`, 
to produce an approximation of a real number.

```
template <typename ReprType = int, int Exponent = 0>
class fixed_point;
```

The intention is to treat integers as the sub-set of fixed-point numbers that have exponent, zero.
As far as practically possible, `fixed_point<T, 0>` should be interchangeable with `T`.

While the most obvious choices for `ReprType` are built-in integers, any integer-like type is acceptable.
The `custom_integer` type described above is an ideal candidate 
and illustrates how `fixed_point` is designed to be extensible.
For example, a u16.16 fixed-point type with high-fidelity rounding characteristics might be expressed as:

```
fixed_point<
    custom_integer<
        rounding::tie_to_odd,  // high-fidelity rounding 
        overflow::undefined,  // don't care what happens when overflow occurs
        32,  // 32 binary digits in total
        false>,  // unsigned
    -16>  // 16 fractional digits
```

### The `elastic` Class Template

The third and final quality required in order to build our general-purpose numeric type is referred to here as elasticity: 
the ability to avoid run-time overflow by increasing the capacity of types resulting from arithmetic operations. 

```
template <int IntegerDigits, int FractionalDigits = 0, typename Archetype = signed>
class elastic;
```

Like `nonnegative` and `negatable`, the return type of arithmetic operations between `elastic` values
are chosen to accommodate the added digits necessary to represent the result.
Consider the example of calculating a hypotenuse:

```
using inout = elastic<7, 8, signed>;  // type used for input and output values, s7.8
inout x = ???, y = ???;  // inputs; s7.8 

auto xx = x * x, yy = y * y;  // intermediates; s14.16
auto sum = xx + yy; // intermediate; s15.16
auto h = sqrt(sum);  // intermediate; u8.8; loss of precision

auto ret = static_cast<elastic<7, 8, signed>>(h);  // output; s7.8; risk of overflow
```

In the above example, required capacity is expanded by the multiply and add operations, 
contracted by the square root operation and then reset by the cast. 
These actions are analogous to the stretching, relaxing and snapping of an elastic band. 

The `Archetype` parameter is essentially a hint at the kind of type that will be employed by `elastic`. 
However, it may not be the type that is actually used. 
Where `signed` is given, any signed, built-in type may be used to store the value.
Similarly, providing `unsigned` will ensure that unsigned, built-in types are used instead.
However, certain operations, such as negation, will transition the result from an unsigned to a signed type.

Elasticity is the defining feature of `nonnegative` and `negatable`. 
Inevitably, they - and `elastic` - rely on fixed-point arithmetic for their implementation.
In the case of `elastic` type, that need can be met using a private `fixed_point` member variable. 
In this way, `elastic` focuses on compile-time overflow handling and nothing else, 
leading to a cleaner interface, a simpler implementation 
and the opportunity to avoid the vast majority of the compile-time cost.

The `elastic` type claims to avoid overflow but there are two notable exceptions to that claim.

Firstly, some operations such as square root and division 
produce irrational or repeating rational numbers which exceed the prescribed number of digits.
This is an inevitable consequence of storing values as a fixed-size single binary value.

Secondly, negation of the most negative value causes positive overflow.
This specific criticism is addressed below.

### Composition

Putting all of these three components together, it is possible to express numeric types with a wide range of properties. 

A high-precision, exception-throwing, (but slow) real-number type might be

```
elastic<7, 8, integer<rounding::tie_to_odd, overflow::exception>>
```

which privately uses `fixed_point<integer<rounding::tie_to_odd, overflow::exception>, -8>`.

A faster (but error prone and imprecise) type might be

```
elastic<7, 8, int>;
```

which privately uses `fixed_point<int16_t, -8>` and requires the inclusion of significantly less template-heavy header code.

A fully customizable type of the kind proposed in [P0106](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0106r0.html) might look like:

```
template <int Range, int Resolution, round Crnd = rounding::tie_to_odd, overflow Covf = overflow::exception>
using negatable = elastic<Range, - Resolution, custom_integer<Crnd, Covf>>;
```

### Archetype Requirements

The integer-like type passed into the `Archetype` parameter of `elastic` 
must be capable of basic arithmetic operations 
as well as conversions between built-in integer and floating-point types. 
It must also support bit-shift and comparison operations.

In addition to these operators, `Archetype` needs for certain type traits to be specialized. 
Ideally, standard traits which describe built-in types would be permitted to be specialized for this purpose.
Existing traits include: `is_signed` and `is_unsigned`, `make_signed` and `make_unsigned`, `is_integral` and `common_type`.

Two additional traits that are needed involve the ability to get and set the width of a type in bits. 
They could take the following form

```c++
template <class Type>
struct width;

template <class Type, unsigned NumBits>
struct set_width;
```

such that `width<set_width<T, S>::type>::value >= S)` always holds.
The templates would come ready-specialized for built-in types such that `set_width<int, 16>::type` would produce `int16_t`
and `width<uint64_t>::value` would equal `64`. 
They could be specialized for any custom type in order to make that type suitable for use with `elastic`.
Notably, while it might not be possible to resize built-ins to types that are larger than `long long`,
no similar limit would be placed on user-defined integers.

## Outstanding Feedback

### Non-Base-2 Numbers

It has been proposed that fixed-point arithmetic be extended beyond binary to any base. 

It is unclear whether this means for some yet-to-be standardized built-in types which are decimal, octagonal etc. 
or whether it is proposed for use with binary integer types, using base-ten for the Exponent parameter of `fixed_point`.

There are concerns that the machine-level efficiency design goals of `fixed_point` might not be attainable without the use of bit-shift operations. 
This would lose a significant section of the intended audience for all fixed-point types discussed here.

The method of specifying the base of a fixed-point type in a user-friendly way is a matter of concern. 
Accessibility of the existing API by the intended audience is already prohibitive. 
At the very least the proposed types would need to be moved to type aliases such as

```
using binary_fixed_point = fixed_point<int, -8, 2>;
```

where 2 is an additional parameter signifying the base of the exponent.

While the appeal of any-base exponents is limited and the added complexity to the API would be far-reaching. 
It is by no means certain that this goal is unattainable. 
Further clarification and assurances regarding performance are welcomed.

### Most Negative Number

One feature of the types from [P0106](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0106r0.html) which is lacking in [P0037](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0037r1.html) is the exclusion of the most negative number. 
For example, the range of `int8_t` is [127, -128]. 
Here, the most negative is -128 and it has no expressible positive equivalent. 
This makes it difficult to implement compile-time overflow avoidance in combination with negating operations. 
As a result, it becomes a problem for `elastic` when used with built-in types which have no run-time error checking.

However, such types are typically used by those who are more concerned with performance than with errors creeping in.
It is conceivable that a custom integer type which is dedicated to catching only this overflow case 
can be implemented and used with the existing model.

### The Overreach of `fixed_point`

When a direct comparison is made between the types in [P0106](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0106r0.html) and [P0037](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0037r1.html), it is clear that `fixed_point` lacks a great deal of functionality. 
This is because the focus of `fixed_point` is far more narrow. However, a number of inappropriate additions have been made to the API in a mistaken attempt to address this perceived shortfall.

Examples of features of [P0037](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0037r1.html) which should be replaced by functionality in the elastic type are the demote and promote functions: `demote`, `promote`, `promote_reciprocal`, `promote_square`, `promote_add`, `promote_sub`, `promote_multiply`, `promote_divide`. 
The equivalent trunc functions are also of questionable value.

It is quite possible that the only named functions required by `fixed_point` are: `negate`, `add`, `subtract`, `multiply` and `divide`. 
These functions would take - as their first template parameter - the result type. 
They could be used to reproduce all of the `trunc_` and `promot_` functions - as well as all of the arithmetic operators of the `elastic` type.

## Acknowledgements

Focus group: Lawrence Crowl, Brian Fitzgerald and Marco Foco.