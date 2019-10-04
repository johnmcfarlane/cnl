
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_INTEGER_DECLARATION_H)
#define CNL_IMPL_SCALED_INTEGER_DECLARATION_H

#include "../power.h"
#include "../../limits.h"

/// compositional numeric library
namespace cnl {
    /// \brief literal real number approximation that uses fixed-point arithmetic
    /// \headerfile cnl/scaled_integer.h
    ///
    /// \tparam Rep the underlying type used to represent the value; defaults to `int`
    /// \tparam Exponent the exponent used to scale the integer value; defaults to `0`
    /// \tparam Radix the base used to scale the integer value; defaults to radix of `Rep`
    ///
    /// Uses an integer to approximate a real number.
    /// Scales the integer by `pow(Radix, Exponent)` to produce the scaled number.
    /// By default, represents an `int` with no fractional digits using binary arithmetic.
    ///
    /// \note Currently, only binary support and partial decimal support is provided.
    ///
    /// \par Examples
    ///
    /// To define a fixed-point value 1 byte in size with a sign bit, 4 integer bits and 3 fractional bits:
    /// \snippet snippets.cpp define a scaled_integer value

    template<typename Rep = int, class Scale = power<>>
    class scaled_integer;
}

#endif  // CNL_IMPL_SCALED_INTEGER_DECLARATION_H
