
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief definition of `cnl::scaled_integer` type

#if !defined(CNL_IMPL_SCALED_INTEGER_DEFINITION_H)
#define CNL_IMPL_SCALED_INTEGER_DEFINITION_H

#include "../../integer.h"
#include "../scaled/is_tag.h"
#include "../scaled/power/definition.h"
#include "../scaled/power/is_scaled_tag.h"
#include "../scaled/quasi_exact.h"
#include "../wrapper.h"

#include <algorithm>

#define CNL_IMPL_DEFAULT_SCALED_INTEGER_SCALE power

/// compositional numeric library
namespace cnl {
    /// \brief literal real number approximation that uses fixed-point arithmetic
    /// \headerfile cnl/scaled_integer.h
    ///
    /// \tparam Rep the underlying type used to represent the value; defaults to `int`
    /// \tparam Scale the scale of the value represented with `Rep`; defaults to \ref CNL_IMPL_DEFAULT_SCALED_INTEGER_SCALE
    ///
    /// Uses an integer to approximate a real number.
    /// Scales the integer by a factor specified by `Scale` to produce the scaled number.
    /// By default, represents an `int` with no fractional digits using binary arithmetic.
    ///
    /// \note Currently, only binary support and partial decimal support is tested.
    ///
    /// \note The type makes no effort to avoid errors resulting from overflow or underflow.
    /// Consider specializing with:
    /// * \ref rounding_integer to reduce precision loss;
    /// * \ref elastic_integer to avoid out-of-range errors;
    /// * \ref overflow_integer to detect out-of-range errors; and
    /// * \ref static_integer to combine all of the above.
    ///
    /// \par Examples
    ///
    /// To define a fixed-point value 1 byte in size with a sign bit, 4 integer bits and 3
    /// fractional bits: \snippet snippets.cpp define a scaled_integer value

#if defined(__GNUG__) && !defined(__clang__)
    template<integer Rep = int, scaled_tag Scale = CNL_IMPL_DEFAULT_SCALED_INTEGER_SCALE<>>
#else
    template<integer Rep = int, class Scale = CNL_IMPL_DEFAULT_SCALED_INTEGER_SCALE<>>
#endif
    using scaled_integer = _impl::wrapper<Rep, Scale>;

    namespace _impl {
        template<typename Rep, typename Scale>
        [[nodiscard]] constexpr auto not_scaled_integer(
                scaled_integer<Rep, Scale> const& f)
        {
            return _impl::to_rep(f);
        }
    }
}

#endif  // CNL_IMPL_SCALED_INTEGER_DEFINITION_H
