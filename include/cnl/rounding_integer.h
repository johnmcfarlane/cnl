
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_ROUNDING_INTEGER_H)
#define CNL_ROUNDING_INTEGER_H

#include "_impl/num_traits/is_composite.h"
#include "_impl/num_traits/rep_of.h"
#include "_impl/num_traits/rounding.h"
#include "_impl/num_traits/set_rounding.h"
#include "_impl/num_traits/to_rep.h"
#include "_impl/operators/tagged.h"
#include "_impl/rounding.h"
#include "_impl/rounding/convert_operator.h"
#include "_impl/rounding/is_rounding_tag.h"
#include "_impl/rounding/nearest_rounding_tag.h"
#include "_impl/wrapper.h"

#include <ostream>
#include <type_traits>

/// compositional numeric library
namespace cnl {
    /// \brief An integer which detects overflow.
    template<typename Rep = int, rounding_tag Tag = nearest_rounding_tag>
    using rounding_integer = _impl::wrapper<Rep, Tag>;

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::rounding

    /// \cond
    template<_impl::wrapped Number>
    requires(!_impl::is_rounding_tag<typename Number::tag>::value) struct rounding<Number>
        : rounding<typename Number::rep> {
    };
    /// \endcond

    template<typename Rep, rounding_tag Tag>
    struct rounding<_impl::wrapper<Rep, Tag>>
        : std::type_identity<Tag> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::_impl::set_rounding

    template<typename Number, rounding_tag Tag>
    struct set_rounding<
            Number, Tag,
            _impl::enable_if_t<is_composite<Number>::value && !_impl::is_wrapper<Number>>>
        : std::type_identity<
                  _impl::set_rep_t<Number, set_rounding_t<_impl::rep_of_t<Number>, Tag>>> {
    };

    template<int Digits, class Rep, rounding_tag Tag>
            struct scale < Digits,
            2, _impl::wrapper<Rep, Tag>,
            _impl::enable_if_t<Digits<0>>
        : _impl::default_scale<Digits, 2, _impl::wrapper<Rep, Tag>> {
    };

    template<int Digits, int Radix, class Rep, rounding_tag Tag>
    struct scale<
            Digits, Radix, _impl::wrapper<Rep, Tag>,
            _impl::enable_if_t<0 <= Digits>> {
        [[nodiscard]] constexpr auto operator()(_impl::wrapper<Rep, Tag> const& s) const
        {
            return _impl::from_rep<_impl::wrapper<Rep, Tag>>(
                    scale<Digits, Radix, Rep>{}(_impl::to_rep(s)));
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::set_rep<rounding_integer, Rep>

    template<typename NumberRep, rounding_tag NumberTag, typename Rep>
    struct set_rep<
            _impl::wrapper<NumberRep, NumberTag>, Rep,
            _impl::enable_if_t<!_impl::is_wrapper<Rep>>>
        : std::type_identity<_impl::wrapper<Rep, NumberTag>> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::set_tag<rounding_integer, Tag>

    template<typename NumberRep, rounding_tag NumberTag, tag Tag>
    struct set_tag<_impl::wrapper<NumberRep, NumberTag>, Tag>
        : std::type_identity<_impl::wrapper<NumberRep, Tag>> {
    };
}

#endif  // CNL_ROUNDING_INTEGER_H
