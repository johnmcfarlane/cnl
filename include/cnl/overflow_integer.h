
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief essential definitions related to the `cnl::overflow_integer` type

#if !defined(CNL_OVERFLOW_INT_H)
#define CNL_OVERFLOW_INT_H

#include "overflow.h"

#include "_impl/custom_operator/definition.h"
#include "_impl/custom_operator/native_tag.h"
#include "_impl/custom_operator/tagged.h"
#include "_impl/num_traits/from_value.h"
#include "_impl/num_traits/from_value_recursive.h"
#include "_impl/num_traits/rep_of.h"
#include "_impl/ostream.h"
#include "_impl/wrapper.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    /// \brief An integer for which out-of-range arithmetic results in overflow
    /// \headerfile cnl/overflow_integer.h
    ///
    /// \tparam Rep the underlying type used to represent the value; defaults to `int`
    /// \tparam Tag tag specifying the overflow-handling strategy; defaults to \ref undefined_overflow_tag
    ///
    /// \sa native_overflow_tag, saturated_overflow_tag, trapping_overflow_tag, undefined_overflow_tag

    template<typename Rep = int, overflow_tag Tag = undefined_overflow_tag>
    using overflow_integer = _impl::wrapper<Rep, Tag>;

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::scale<..., overflow_integer<>>

    template<int Digits, int Radix, typename Rep, overflow_tag Tag>
    struct scale<Digits, Radix, _impl::wrapper<Rep, Tag>> {
    private:
        using value_type = _impl::wrapper<Rep, Tag>;

    public:
        [[nodiscard]] constexpr auto operator()(value_type const& s) const -> decltype(_impl::from_rep<value_type>(_impl::scale<Digits, Radix>(_impl::to_rep(s))))
        {
            return _impl::default_scale<Digits, Radix, value_type>{}(s);
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::set_rep<Rep, OverflowTag>

    // when an _impl::wrapper wraps a non-_impl::wrapper
    /// \cond
    template<typename NumberRep, overflow_tag NumberTag, typename Rep>
    requires(!_impl::is_wrapper<Rep>) struct set_rep<_impl::wrapper<NumberRep, NumberTag>, Rep>
        : std::type_identity<_impl::wrapper<Rep, NumberTag>> {
    };
    /// \endcond

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::set_tag<overflow_integer, OverflowTag>

    template<typename NumberRep, overflow_tag NumberTag, tag Tag>
    struct set_tag<_impl::wrapper<NumberRep, NumberTag>, Tag>
        : std::type_identity<_impl::wrapper<NumberRep, Tag>> {
    };
}

#endif  // CNL_OVERFLOW_INT_H
