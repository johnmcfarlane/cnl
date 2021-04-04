
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_STATIC_NUBER_H)
#define CNL_STATIC_NUBER_H

/// \file
/// \brief file containing definitions related to \ref cnl::static_number

#include "_impl/static_integer.h"
#include "_impl/wrapper/tag_of.h"
#include "scaled_integer.h"

/// compositional numeric library
namespace cnl {
    /// \brief a general-purpose fixed-point real number type
    ///
    /// \tparam Digits number of binary digits
    /// \tparam Exponent the exponent used to scale the integer value
    /// \tparam OverflowTag behavior exhibited on out-of-range conditions
    /// \tparam RoundingTag behavior exhibited on precision loss
    /// \tparam Narrowest narrowest integer with which to represent the value
    ///
    /// \sa static_integer
    template<
            int Digits, int Exponent = 0, rounding_tag RoundingTag = nearest_rounding_tag,
            overflow_tag OverflowTag = undefined_overflow_tag, class Narrowest = signed>
    using static_number = scaled_integer<
            _impl::static_integer<Digits, RoundingTag, OverflowTag, Narrowest>, power<Exponent>>;

    /// \brief constructs a static_number from a given variable
    template<
            rounding_tag RoundingTag = nearest_rounding_tag, overflow_tag OverflowTag = undefined_overflow_tag,
            class Narrowest = signed, class Input = int>

    [[nodiscard]] constexpr auto make_static_number(Input const& input)
            -> static_number<numeric_limits<Input>::digits, 0, RoundingTag, OverflowTag, Narrowest>
    {
        return input;
    }

    /// \brief constructs a static_number from a given constant value
    template<
            rounding_tag RoundingTag = _impl::tag_of_t<rounding_integer<>>,
            overflow_tag OverflowTag = _impl::tag_of_t<overflow_integer<>>, class Narrowest = int,
            class Input = int, CNL_IMPL_CONSTANT_VALUE_TYPE Value>
    [[nodiscard]] constexpr auto make_static_number(constant<Value> const&) -> static_number<
            _impl::used_digits(Value) - trailing_bits(Value), trailing_bits(Value), RoundingTag,
            OverflowTag, Narrowest>
    {
        return constant<Value>{};
    }
}

#endif  // CNL_STATIC_NUBER_H
