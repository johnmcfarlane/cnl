
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `cnl::overflow_integer` type

#if !defined(CNL_OVERFLOW_INT_H)
#define CNL_OVERFLOW_INT_H

#include "overflow.h"

#include "_impl/num_traits/from_value.h"
#include "_impl/num_traits/from_value_recursive.h"
#include "_impl/num_traits/rep_of.h"
#include "_impl/number.h"
#include "_impl/operators/generic.h"
#include "_impl/operators/native_tag.h"
#include "_impl/operators/tagged.h"
#include "_impl/ostream.h"
#include "_impl/type_traits/enable_if.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    /// \brief An integer which detects overflow.
    template<typename Rep = int, overflow_tag Tag = undefined_overflow_tag>
    using overflow_integer = _impl::number<Rep, Tag>;

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::scale<..., overflow_integer<>>

    template<int Digits, int Radix, typename Rep, overflow_tag Tag>
    struct scale<Digits, Radix, _impl::number<Rep, Tag>> {
        using _value_type = _impl::number<Rep, Tag>;

        CNL_NODISCARD constexpr auto operator()(_value_type const& s) const -> decltype(
                _impl::from_rep<_value_type>(_impl::scale<Digits, Radix>(_impl::to_rep(s))))
        {
            return _impl::default_scale<Digits, Radix, _value_type>{}(s);
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::set_rep<Rep, OverflowTag>

    // when an _impl::number wraps a non-_impl::number
    template<typename NumberRep, overflow_tag NumberTag, typename Rep>
    struct set_rep<
            _impl::number<NumberRep, NumberTag>, Rep,
            _impl::enable_if_t<!_impl::is_number<Rep>>>
        : std::type_identity<_impl::number<Rep, NumberTag>> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::set_tag<overflow_integer, OverflowTag>

    template<typename NumberRep, overflow_tag NumberTag, tag Tag>
    struct set_tag<_impl::number<NumberRep, NumberTag>, Tag>
        : std::type_identity<_impl::number<NumberRep, Tag>> {
    };
}

#endif  // CNL_OVERFLOW_INT_H
