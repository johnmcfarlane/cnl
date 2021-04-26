
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_STATIC_NUBER_H)
#define CNL_STATIC_NUBER_H

/// \file
/// \brief file containing definitions related to \ref cnl::static_number

#define CNL_IMPL_DEFAULT_STATIC_NUMBER_SCALE quasi_exact

#include "_impl/scaled/power.h"
#include "_impl/scaled/quasi_exact.h"
#include "_impl/static_integer.h"
#include "_impl/wrapper/tag_of.h"
#include "integer.h"
#include "scaled_integer.h"

/// compositional numeric library
namespace cnl {
    /// \brief a general-purpose fixed-point real number type
    ///
    /// \tparam Digits number of binary digits
    /// \tparam Scale the scale of the integer; defaults to \ref CNL_IMPL_DEFAULT_STATIC_NUMBER_SCALE
    /// \tparam OverflowTag behavior exhibited on out-of-range conditions
    /// \tparam RoundingTag behavior exhibited on precision loss
    /// \tparam Narrowest narrowest integer with which to represent the value
    ///
    /// \sa static_integer
    template<
            int Digits, scaled_tag Scale = CNL_IMPL_DEFAULT_STATIC_NUMBER_SCALE<>, rounding_tag RoundingTag = nearest_rounding_tag,
            overflow_tag OverflowTag = undefined_overflow_tag, integer Narrowest = int>
    using static_number = scaled_integer<
            _impl::static_integer<Digits, RoundingTag, OverflowTag, Narrowest>, Scale>;

    /// \brief constructs a static_number from a given variable
    template<
            rounding_tag RoundingTag = nearest_rounding_tag, overflow_tag OverflowTag = undefined_overflow_tag,
            integer Narrowest = int, class Input = int>
    [[nodiscard]] constexpr auto make_static_number(Input const& input)
            -> static_number<numeric_limits<Input>::digits, CNL_IMPL_DEFAULT_STATIC_NUMBER_SCALE<>, RoundingTag, OverflowTag, Narrowest>
    {
        return input;
    }

    /// \brief constructs a static_number from a given constant value
    template<
            rounding_tag RoundingTag = _impl::tag_of_t<rounding_integer<>>,
            overflow_tag OverflowTag = _impl::tag_of_t<overflow_integer<>>, integer Narrowest = int,
            class Input = int, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    [[nodiscard]] constexpr auto make_static_number(constant<Value> const&) -> static_number<
            _impl::used_digits(Value) - trailing_bits(Value), CNL_IMPL_DEFAULT_STATIC_NUMBER_SCALE<trailing_bits(Value)>, RoundingTag,
            OverflowTag, Narrowest>
    {
        return constant<Value>{};
    }
}

#endif  // CNL_STATIC_NUBER_H
