
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
 * The Compositional Numeric Library (CNL) is a C++ library of fixed-precision numeric classes which enhance integers to
 * deliver safer, simpler, cheaper arithmetic types.
 *
 * CNL improves on numbers the same way that the STL improves on arrays and smart pointers improve on pointers.
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
 * - [scaled_integer](@ref cnl::scaled_integer) - scales integers by any exponent and base with zero-overhead and minimal
 * precision loss;
 * - [elastic_integer](@ref cnl::elastic_integer) - prevents overflow at compile-time by generalizing promotion rules;
 * - [overflow_integer](@ref cnl::overflow_integer) - handles integer overflow at run-time;
 * - [rounding_integer](@ref cnl::rounding_integer) - improves rounding behavior of integers;
 * - [wide_integer](@ref cnl::wide_integer) - provides integers wider than 64 and 128 bits using multi-word arithmetic;
 * - [fraction](@ref cnl::fraction) - low-level dividend/divisor pair aids refined division handling and
 * - [constant](@ref cnl::constant) - a numerics-friendly alternative to @ref std::integral_constant.
 *
 * Each of these types solves a single problem when used alone.
 *
 * \subsection Composite_Types Composite Types
 *
 * In combination, the core types produce composites which address multiple concerns.
 *
 * Provided composite types include:
 * - [elastic_scaled_integer](@ref cnl::elastic_scaled_integer) ([scaled_integer](@ref cnl::scaled_integer) and
 *   [elastic_integer](@ref cnl::elastic_integer)) - real-number approximation which uses promotion to avoid overflow;
 * - [static_integer](@ref cnl::static_integer) ([rounding_integer](@ref cnl::rounding_integer),
 *   [overflow_integer](@ref cnl::overflow_integer), [elastic_integer](@ref cnl::elastic_integer) and
 *   [wide_integer](@ref cnl::wide_integer)) - fully-featured safe, accurate integer type and
 * - [static_number](@ref cnl::static_number) ([scaled_integer](@ref cnl::scaled_integer) and
 *   [static_integer](@ref cnl::static_integer)) - a fully-featured safe, accurate real number approximation.
 *
 * Many more combinations are possible. For example:
 * - `safe_integer` ([overflow_integer](@ref cnl::overflow_integer) and [elastic_integer](@ref cnl::elastic_integer));
 * - `wide_elastic_integer` ([elastic_integer](@ref cnl::elastic_integer) and [wide_integer](@ref cnl::wide_integer))
 * and
 * - `safe_fraction` ([fraction](@ref cnl::fraction) and `safe_integer`).
 *
 * \section Examples
 *
 * The following examples can be found in the
 * [test suite](../../test/unit/index.cpp).
 *
 * \subsection Declaration
 *
 * The [scaled_integer](@ref cnl::scaled_integer) type adds a scaling exponent to integers. This enables the integer to
 * represent very big or very small values.
 *
 * \snippet index.cpp declaration example
 *
 *
 * \subsection Arithmetic_Operators Arithmetic Operators
 *
 * Specializations of [scaled_integer](@ref cnl::scaled_integer) behave a lot like native C/C++ numeric types.
 * Operators are designed to behave in an way which is both predictable and efficient.
 *
 * \snippet index.cpp basic arithmetic example
 *
 * \subsection Arithmetic_Functions Arithmetic Functions
 * But one size does not fit all.
 * Different applications of the same operation might call for different trade-offs between storage, precision, safety and speed.
 * Named functions provide fine-tuned control over arithmetic results.
 *
 * \snippet index.cpp advanced arithmetic example
 *
 * \subsection Extensible
 *
 * Because one size does not fit all, [scaled_integer](@ref cnl::scaled_integer) is designed to make it easy to tailor new arithmetic types.
 * The [elastic_scaled_integer](@ref cnl::elastic_scaled_integer) type illustrates this.
 * As each calculation requires more digits, so the results of [elastic_scaled_integer](@ref cnl::elastic_scaled_integer) operations allocate more storage.
 *
 * \snippet index.cpp elastic example
 *
 * */

#if !defined(CNL_ALL_H)
#define CNL_ALL_H

#include "bit.h"
#include "cmath.h"
#include "constant.h"
#include "cstdint.h"
#include "elastic_fixed_point.h"
#include "elastic_integer.h"
#include "elastic_scaled_integer.h"
#include "fixed_point.h"
#include "fraction.h"
#include "limits.h"  // NOLINT(modernize-deprecated-headers,  hicpp-deprecated-headers)
#include "num_traits.h"
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
