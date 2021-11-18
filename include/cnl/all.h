
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief include-all header for the CNL numeric library;
/// contains front page of user manual

/*! \mainpage User Manual
 *
 * \section Introduction
 *
 * The Compositional Numeric Library (CNL) is a C++ library of fixed-precision numeric classes which
 * enhance integers to deliver safer, simpler, cheaper arithmetic types.
 *
 * CNL improves on numbers the same way that the STL improves on arrays and smart pointers improve
 * on pointers.
 *
 * Uses include
 * - simulations that rely on precision, determinism, and predictability,
 * - financial applications handling decimal fractions and
 * - resource-critical systems where floating-point arithmetic is too costly.
 *
 * The CNL project is published under the Boost licence and can be found online at
 * [github.com/johnmcfarlane/cnl](https://github.com/johnmcfarlane/cnl).
 *
 * \section Components
 *
 * \subsection Core_Components Core Components
 *
 * The core numeric types of CNL are:
 * - [scaled_integer](\ref cnl::scaled_integer) - scales integers by any exponent and base with
 * zero-overhead and minimal precision loss;
 * - [elastic_integer](\ref cnl::elastic_integer) - prevents overflow at compile-time by
 * generalizing promotion rules;
 * - [overflow_integer](\ref cnl::overflow_integer) - handles integer overflow at runtime;
 * - [rounding_integer](\ref cnl::rounding_integer) - improves rounding behavior of integers;
 * - [wide_integer](\ref cnl::wide_integer) - provides integers wider than 64 and 128 bits using
 * multi-word arithmetic;
 * - [fraction](\ref cnl::fraction) - low-level dividend/divisor pair aids refined division handling
 * and
 * - [constant](\ref cnl::constant) - a numerics-friendly alternative to \ref std::integral_constant.
 *
 * Each of these types solves a single problem when used alone.
 *
 * \subsection Composite_Types Composite Types
 *
 * In combination, the core types produce composites which address multiple concerns.
 *
 * Provided composite types include:
 * - [elastic_scaled_integer](\ref cnl::elastic_scaled_integer)
 *   ([scaled_integer](\ref cnl::scaled_integer) and [elastic_integer](\ref cnl::elastic_integer)) -
 *   real-number approximation which uses promotion to avoid overflow;
 * - [static_integer](\ref cnl::static_integer) ([rounding_integer](\ref cnl::rounding_integer),
 *   [overflow_integer](\ref cnl::overflow_integer), [elastic_integer](\ref cnl::elastic_integer)
 * and [wide_integer](\ref cnl::wide_integer)) - fully-featured safe, accurate integer type and
 * - [static_number](\ref cnl::static_number) ([scaled_integer](\ref cnl::scaled_integer) and
 *   [static_integer](\ref cnl::static_integer)) - a fully-featured safe, accurate real number
 * approximation.
 *
 * Many more combinations are possible. For example:
 * - `safe_integer` ([overflow_integer](\ref cnl::overflow_integer) and
 *   [elastic_integer](\ref cnl::elastic_integer));
 * - `wide_elastic_integer` ([elastic_integer](\ref cnl::elastic_integer) and
 *   [wide_integer](\ref cnl::wide_integer)) and
 * - `safe_fraction` ([fraction](\ref cnl::fraction) and `safe_integer`).
 *
 * \section Examples
 *
 * The following examples can be found in the
 * [test suite](../../test/unit/index.cpp).
 *
 * \subsection Declaration
 *
 * The [scaled_integer](\ref cnl::scaled_integer) type adds a scaling exponent to integers. This
 * enables the integer to represent very big or very small values.
 *
 * \snippet index.cpp declaration example
 *
 *
 * \subsection Arithmetic_Operators Arithmetic Operators
 *
 * Specializations of [scaled_integer](\ref cnl::scaled_integer) behave a lot like native C/C++
 * numeric types. Operators are designed to behave in an way which is both predictable and
 * efficient.
 *
 * \snippet index.cpp basic arithmetic example
 *
 * \subsection Arithmetic_Functions Arithmetic Functions
 * But one size does not fit all.
 * Different applications of the same operation might call for different trade-offs between storage,
 * precision, safety and speed. Named functions provide fine-tuned control over arithmetic results.
 *
 * \snippet index.cpp advanced arithmetic example
 *
 * \subsection Extensible
 *
 * Because one size does not fit all, [scaled_integer](\ref cnl::scaled_integer) is designed to make
 * it easy to tailor new arithmetic types. The
 * [elastic_scaled_integer](\ref cnl::elastic_scaled_integer) type illustrates this. As each
 * calculation requires more digits, so the results of
 * [elastic_scaled_integer](\ref cnl::elastic_scaled_integer) operations allocate more storage.
 *
 * \snippet index.cpp elastic example
 *
 * \section FAQ
 *
 * \subsection overflow Fixed-point Overflow
 *
 * <b>Q:</b> Why do calculations using \ref cnl::scaled_integer result in completely wrong results?
 *
 * <b>A:</b> Most surprises reported by CNL users occur when a \ref cnl::scaled_integer
 * value exceeds the range that its type can represent. This is normally caused by arithmetic
 * operations (especially multiplication) and from conversion between different scales. It
 * typically results in 'modulo' or overflow behavior.
 *
 * Consider the multiplication of 1*1. What could possibly go wrong?
 *
 * \snippet ub.cpp scaled_integer overflow example 1
 *
 * \ref cnl::scaled_integer does nothing more here than perform integer multiplication.
 * But the product is too great to be stored in an `int` (on a typical system).
 * Here is the equivalent operation being performed directly on the `int`:
 *
 * \snippet ub.cpp scaled_integer overflow example 2
 *
 * This value is too great to be stored in a 32-bit integer. In both cases overflow will occur
 * and the result will not be valid.
 *
 * <b>Q:</b> Why do calculations using \ref cnl::scaled_integer result in a `static_assert` error?
 *
 * Errors with the message "attempted operation will result in overflow" may be the result
 * of conversion between types whose ranges do not intersect as described above.
 * For example, when converting from a 32-bit \ref cnl::scaled_integer to a 64-bit
 * \ref cnl::scaled_integer, you may find that the 32-bit number is being scaled up or down by 32
 * bits before being converted to 64-bits. This would result in overflow.
 * Read on for suggested solutions...
 *
 * <b>Q:</b> Why doesn't \ref cnl::scaled_integer prevent/avoid/detect overflow?
 *
 * <b>A:</b> CNL provides a library of components which each address a single concern.
 * The concern of \ref cnl::scaled_integer is the approximation of real numbers with integers.
 * It does this as efficiently as the chosen integer type allows. In the case of
 * `int`, this is very efficient, but without safety checks.
 *
 * There are several solutions with different tradeoffs including:
 * * sanitizers, which can detect many such errors at runtime ([example](https://godbolt.org/z/GdY6ce));
 * * replace `int` with \ref cnl::overflow_integer to detect all such errors at runtime ([example](https://godbolt.org/z/sx3bvc));
 * * replace `int` with \ref cnl::elastic_integer to avoid many such errors at compile-time ([example](https://godbolt.org/z/Knfn39));
 * * consider other \link Composite_Types Composite Types \endlink to find a good balance between efficiency, safety and correctness.
 *
 * <b>Q:</b> Why do some overflow errors only show up / not show up at run-time?
 *
 * <b>A:</b> The preferred methods for signaling overflow errors are by trapping them
 * and halting the program, by invoking undefined behavior (UB), or the combination of
 * trapping UB (using a sanitizer).
 * Trapping inevitably has to occur at run-time because the compiler cannot possibly know
 * all the values that a program might compute...
 *
 * ...except when an expression is evaluated at compile time, e.g. a `constexpr` value.
 * Such an evaluate is not allowed to exhibit UB,
 * and therefore the compiler (helpfully) rejects it.
 *
 * In the case where CNL is configured to not signal overflow, overflow may go undetected.
 * And further, in the case that the overflow is UB, the program is defective and \b must be fixed.
 *
 * \subsection division Fixed-point Division
 *
 * <b>Q:</b> Why aren't the results of \ref cnl::scaled_integer division more precise?
 *
 * <b>A:</b> Integer and floating-point division are fundamentally different.
 * Being integer-based, \ref cnl::scaled_integer uses integer division in its '`/`' operator.
 *
 * This is surprising to users who expect fixed-point types to behave like floating-point types.
 * But the floating-point '`/`' operator returns results with a variable exponent — something a
 * fixed-point type cannot do.
 *
 * In comparison, the integer '`/`' operator yields an integer quotient
 * while the '`%`' operator provides the remainder.
 * This gives lossless results, provided the remainder is taken into account.
 * Integer division is more efficient and more versatile.
 * It's also more intuitive for applications involving discrete values, such as denominations:
 *
 * \snippet snippets.cpp scaled_integer division example
 *
 * Output:
 *
 * > Q: If I have €5 and candles cost €1.1, how many candles can I buy?<br/>A: I get 4 candles and €.6 change.
 *
 * You can read more on the subject in the paper,
 * [Multiplication and division of fixed-point numbers](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1368r1.html)
 * by Davis Herring.
 *
 * <b>Q:</b> OK, but what if I'm dealing with continuous values?
 *
 * <b>A:</b> For now, divide numbers using the \ref cnl::quotient API,
 * which makes a best guess about the exponent you want,
 * or consider avoiding division altogether by using the \ref cnl::fraction type.
 *
 * Due to popular demand an alternative fixed-point type, which implements 'quasi-exact' division
 * is planned for a future revision of the library.
 *
 * \subsection limits Elastic Limits
 *
 * <b>Q:</b> Why doesn't the `Digits` parameter of types such as \ref cnl::elastic_integer
 * include the 'sign' bit?
 *
 * <b>A:</b> Types, \ref cnl::elastic_integer, \ref cnl::elastic_scaled_integer,
 * \ref cnl::static_integer and \ref cnl::static_number all have a `Digits` parameter which
 * specifies how many binary digits the type stores. This corresponds to the number if bits
 * in the value, *independent of the negated two's complement MSB*. It is the same value expressed
 * in the `digits` member of \ref std::numeric_limits. It is *not* the same as the width value
 * found in types such as \ref std::int32_t which are defined in `<cstdint>`.
 *
 * One reason to choose digits instead of width is that this value indicates the range of a number
 * consistently between signed and unsigned types. While confusing to anyone who is used to
 * dealing in widths (e.g. \ref std::int32_t vs \ref std::uint32_t), it makes the implementation
 * simpler and reduces the risk of overflow errors.
 *
 * */

#if !defined(CNL_ALL_H)
#define CNL_ALL_H

#include "arithmetic.h"
#include "bit.h"
#include "cmath.h"
#include "constant.h"
#include "cstdint.h"
#include "elastic_integer.h"
#include "elastic_scaled_integer.h"
#include "fixed_point.h"
#include "floating_point.h"
#include "fraction.h"
#include "integer.h"
#include "num_traits.h"
#include "number.h"
#include "numeric.h"
#include "overflow.h"
#include "overflow_integer.h"
#include "rounding.h"
#include "rounding_integer.h"
#include "scaled_integer.h"
#include "static_integer.h"
#include "static_number.h"
#include "type_traits.h"
#include "wide_integer.h"

#endif  // CNL_ALL_H
