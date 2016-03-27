# Elastic Number Type

## Introduction

This paper addresses feedback provided in response to the proposal in P0037. Particular attention is paid to comparisons between P0037 and the similar proposal in P0106.

## Background

### Existing Papers

In P0037 [[1]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0037r1.html), 
the author introduces a binary fixed-point arithmetic type, `fixed_point`, 
which approximates real numbers by storing an integer value that is shifted up or down by a fixed number of bits. 
It automates laborious, error-prone operations which are routinely performed in situations where floating-point cannot be used. 
It resembles the APIs provided by existing libraries such as fp and fixed-point.

Lawrence Crowl's paper, P0106 [[2]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0106r0.html), 
details a number of numeric types - some of which employ fixed-point arithmetic. 
These types are intended to cater for a wide variety of users by offering a wide range of strategies for handling errors and precision loss.

### Feedback on P0037

Valuable feedback on P0037 and P0106 has been provided by - among others - 
SG6 at Kona in 2015 [[3]](https://groups.google.com/a/isocpp.org/forum/?utm_medium=email&utm_source=footer#!msg/sg14/-Dw-jYbhlAc/E_ifUGPMDgAJ) and 
Jacksonville in 2016 [[4]](https://groups.google.com/a/isocpp.org/forum/?utm_medium=email&utm_source=footer#!msg/sg14/ak-1E9-0PsE/h6Bs_CSCAQAJ) 
as well as SG14 at CppCon 2015 [[5]](https://groups.google.com/a/isocpp.org/forum/#!searchin/sg14/minutes/sg14/ey5o_ZIxSXs/aTLtbKdWBQAJ) and GDC 2016. 

Among the observations of P0037's `fixed_point` type, some reservations were:
* it does not handle overflow;
* it lacks many desired rounding modes;
* it's capacity is limited by the capacity of the integer value used as its representational type;
* it possesses a most negative number whose magnitude is greater than its most positive value;
* it is limited to base two numbers.

A general recommendation was that the proposals in P0037 and P0106 should be combined into a single effort.

## Discussion

Most of the feedback items (including those listed above) are aimed at `fixed_point` when compared to the numeric types detailed in P0106. 
However, it is not the intent of `fixed_point` to address all of the same concerns as those types. 
Its aim is to do one job: to approximate real numbers using integers.
In doing so, it can support many applications of fixed-point arithmetic including - but not limited to -
general-purpose types such as P0106's `nonnegative` and `negatable`
as well as traditional applications such as those described in P0037.

The following section outlines an example architecture to which the `fixed_point` type might apply. 
It describes three types which can be used independently or combined to provide the major features described in P0106.

## Example Numeric Types Using `fixed_point`

### The `custom_integer` Class Template

The `custom_integer` type provides template parameters for fine-tuning rounding style, overflow handling and capacity - 
much in the spirit of the `cardinal` and `integral` types proposed in P0106.
For ease of comparison, it adopts the same `rounding` and `overflow` enumerations
defined in P0106's accompanying paper, P0105 [[6]](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0105r0.html).

``` c++
template <
    rounding Crnd = rounding::native, 
    overflow Covf = overflow::native, 
    int NumDigits = numeric_limits<int>::digits(), 
    bool IsSigned = true>
class custom_integer;
```

When all template parameters are set to defaults, `custom_integer` behaves as closely as possible to `int`. 
More generally, it is intended that `custom_integer` can provide a drop-in replacement for `int` in a variety of applications. 
In this way, the user can choose to replace `custom_integer` with `int` whenever raw run-time or compile-time performance become a concern.

The range of `custom_integer` may exceed `NumDigits` in order to exploit the target architecture. 
For example, when `NumDigits==1`, it is inevitable that the type will be at least a byte in size with 7 or 8 digits. 

It is likely that `custom_integer` can further be decomposed into types which implement custom sizes, rounding modes and overflow strategies. 
Details are beyond the scope of this paper but to illustrate this composition, the following alternative definition of `custom_integer` is given:

``` c++
template <round Crnd, overflow Covf, int NumDigits, bool IsSigned>
using custom_integer = overflow_integer<Covf, rounding_integer<Crnd, sized_integer<NumBytes, IsSigned>>>;
```

### The `fixed_point` Class Template

Unlike the P0106 types, the sole purpose of this type is to approximate real numbers using integers. 
It deliberately avoids solving problems of rounding and of run-time and compile-time overflow handling.

```
template <typename ReprType = int, int Exponent = 0>
class fixed_point;
```

A `fixed_point` specialization is the combination of an integer plus an exponent.
Put another way, an integer is a fixed-point type with an exponent of zero.
To the furthest practical extent type, `fixed_point<T, 0>` should be interchangeable with `T`.

While the most obvious choices for `ReprType` are built-in integers, any conceptually similar type is acceptable.
The `custom_integer` type described above is an ideal candidate for this role. 
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

The third and final feature required in order to build a general-purpose numeric type is referred to here as elasticity: 
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

auto xx =x * x, yy = y * y;  // intermediates; s14.16
auto sum = xx + yy; // intermediate; s15.16
auto h = sqrt(sum);  // intermediate; u8.8; loss of precision

auto ret = static_cast<elastic<7, 8, signed>>(h);  // output; s7.8; risk of overflow
```

In the above example, required capacity is expanded by the multiply and add operations, 
contracted by the square root operation and then reset by cast. 
These actions are analogous to the stretching, relaxing and snapping of an elastic band. 

The `Archetype` parameter is essentially a hint at the kind of type that will be employed by `elastic`. 
However, it may not be the type that is actually used. 
Where `signed` is given, any signed, built-in type may be used to store the value.
Similarly, providing `unsigned` will ensure that unsigned, built-in types are used instead.
However, certain operations, such as *negate*, will transition the result from unsigned to signed types.

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

A high-precision, exception-throwing, (but slow) real-number type might be:

```
elastic<7, 8, integer<rounding::tie_to_odd, overflow::exception>>
```

which privately uses `fixed_point<integer<rounding::tie_to_odd, overflow::exception>, -8>`.

A faster (but error prone and imprecise) type might be

```
elastic<7, 8, int>;
```

which privately uses `fixed_point<int, -8>` and requires the inclusion of significantly less template-heavy header code.

A fully customizable type of the kind proposed in P0106 might look like:

```
template <int Range, int Resolution, round Crnd = rounding::tie_to_odd, overflow Covf = overflow::exception>
using negatable = elastic<Range, - Resolution, integer<Crnd, Covf>>;
```

### Archetype Requirements

The integer-like type passed into the `Archetype` parameter of `elastic` 
must be capable of basic arithmetic operations 
as well as conversions between built-in interger and floating-point types. 
It must also support bit-shift and comparison operations.

In addition to these operators, `Archetype` needs for certain standard type traits to be specialized. 
Existing types include: `is_signed` and `is_unsigned`, `make_signed` and `make_unsigned`, `is_integral` and `common_type`.

An additional trait which is as-yet absent from the standard is one which can statically resize types. 
That is to say, given an integral type, `P`, for which `sizeof(P) == N`, 
there needs to be a way to determine type, `Q`, for which `sizeof(Q) == N*2`. 
`Q` should be of the same 'family' as `P`. For instance, they should both have the same signedness.
For this purpose, a helper type, `resize`, is proposed such that the following compile-time tests can be true:

```
is_same_v<resize_t<int, 2>, int16_t>;
is_same_v<resize_t<unsigned char, 6>, uint64_t>;  // rounding up is allowed but not necessary
is_same_v<
    resize_t<integer<rounding::tie_to_odd, overflow::exception, 4, true>, 8>,
    integer<rounding::tie_to_odd, overflow::exception, 8, true>>;
is_same_v<
    resize_t<fixed_point<uint32_t, -4>, 2>>,
    fixed_point<uint16_t>, -4>>;
```

This functionality is required in order for `elastic` 
to generate types of ever-increasing capacity as subsequent operations require them. 
It is understood that `elastic<*, *, int>` will not stretch beyond the capacity of `int64_t`. 
Users wishing to exceed this capacity must turn to `custom_integer` as an alternative.

## Outstanding Feedback

### Non-Base-2 Numbers

It has been proposed that fixed-point arithmetic be extended beyond binary to any base. 

It is unclear whether this means for some yet-to-be standardized built-in types which are decimal, octagonal etc. 
or whether it is proposed for use with binary integer types, using base-ten for the Exponent parameter of `fixed_point`.

There are concerns that the machine-level efficiency design goals of `fixed_point` might not be attainable without the use of bit-shift operations. 
This would lose a significant section of the intended audience for fixed-point and the other types discussed here.

The method of specifying the base of a fixed-point type in a user-friendly way is a matter of concern. 
Accessibility of the existing API by the intended audience is already prohibitive. 
At the very least the proposed types would need to be moved to types aliases such as:

```
using binary_fixed_point = fixed_point<int, -8, 2>;
```

where 2 is an additional parameter signifying the base of the exponent.

While the appeal of any-base exponents is limited and the added complexity to the API would be far-reaching. 
It is by no means certain that this addition is unattainable. 
Further clarification and assurances regarding performance are welcomed.

### Most Negative Number

One feature of the types from P0106 which is lacking in P0037 is the exclusion of the most negative number. 
For example, the range of `int8_t` is [127, -128]. 
Here, the most negative is -128 and it has no expressible positive equivalent. 
This makes it difficult to implement compile-time overflow avoidance in combination with negation operations. 
As a result, it becomes a problem for `elastic` when used with built-in types which have no run-time error checking.

In general, it seems likely that a solution can be found in cases where correctness is more important than performance. 
Whether there is a zero-cost run-time defence against most negative number remains an open question.

### The Overreach of `fixed_point`

When a direct comparison is made between the types in P0106 and P0037, it is clear that `fixed_point` lacks a great deal of functionality. 
This is because the focus of `fixed_point` is far more narrow. However, a number of innapropriate additions have been made to the API in a mistaken attempt to address this perceived shortfall.

Examples of features of P0037 which should be replaced by functionality in the elastic type are the demote and promote functions: `demote`, `promote`, `promote_reciprocal`, `promote_square`, `promote_add`, `promote_sub`, `promote_multiply`, `promote_divide`. 
The equivalent trunc functions are of questionable value.

It is quite possible that the only named functions required by `fixed_point` are `add`, `subtract`, `multiply`, `divide`. 
These functions would take - as their first template parameter - the result type. 
They could be used to reproduce all of the `trunc_` and `promot_` functions - as well as all of the arithmetic operators of the `elastic` type.

## Acknowledgements

Special thanks to Lawrence Crowl and Marco Foco and to the SG6 and SG14 study groups for invaluable input and ideas.
