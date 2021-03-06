
//          Copyright John McFarlane 2015 - 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_STATIC_INTEGER_H)
#define CNL_STATIC_INTEGER_H

/// \file
/// \brief file containing definitions related to \ref cnl::static_integer

#include "_impl/overflow/is_overflow_tag.h"
#include "_impl/rounding/is_rounding_tag.h"
#include "_impl/static_integer.h"

/// compositional numeric library
namespace cnl {
    /// \brief a general-purpose integer type
    ///
    /// \tparam Digits number of binary digits
    /// \tparam OverflowTag behavior exhibited on out-of-range conditions
    /// \tparam RoundingTag behavior exhibited on precision loss
    /// \tparam Narrowest narrowest integer with which to represent the value
    ///
    /// \sa static_number
    template<
            int Digits = digits_v<int>, rounding_tag RoundingTag = nearest_rounding_tag,
            overflow_tag OverflowTag = undefined_overflow_tag, class Narrowest = int>
    using static_integer = _impl::static_integer<Digits, RoundingTag, OverflowTag, Narrowest>;

    /// \brief constructs a static_integer from a given value
    using _impl::make_static_integer;
}

#endif  // CNL_STATIC_INTEGER_H
