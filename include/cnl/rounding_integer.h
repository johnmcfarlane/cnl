
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_ROUNDING_INTEGER_H)
#define CNL_ROUNDING_INTEGER_H 1

#include "_impl/num_traits/is_composite.h"
#include "_impl/num_traits/rounding.h"
#include "_impl/num_traits/set_rounding.h"
#include "_impl/num_traits/to_rep.h"
#include "_impl/rounding.h"
#include "_impl/operators/tagged.h"
#include "_impl/integer.h"
#include "cnl/_impl/rounding/convert_operator.h"
#include "_impl/rounding/nearest_rounding_tag.h"

/// compositional numeric library
namespace cnl {
    /// \brief An integer which detects overflow.
    template<typename Rep = int, class Tag = nearest_rounding_tag>
    using rounding_integer = _impl::integer<Rep, Tag>;

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::rounding

    template<typename Number>
    struct rounding<Number, _impl::enable_if_t<is_composite<Number>::value&&!_impl::is_integer<Number>::value>>
            : rounding<_impl::to_rep_t<Number>> {
    };

    template<typename Rep, class Tag>
    struct rounding<_impl::integer<Rep, Tag>, _impl::enable_if_t<!_impl::is_rounding_tag<Tag>::value>>
            : rounding<Rep> {
    };

    template<typename Rep, class RoundingTag>
    struct rounding<_impl::integer<Rep, RoundingTag>, _impl::enable_if_t<_impl::is_rounding_tag<RoundingTag>::value>>
        : _impl::type_identity<RoundingTag> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::_impl::set_rounding

    template<typename Number, class RoundingTag>
    struct set_rounding<Number, RoundingTag, _impl::enable_if_t<
            is_composite<Number>::value&&!_impl::is_integer<Number>::value>>
            : _impl::type_identity<
                    _impl::from_rep_t<
                            Number,
                            set_rounding_t<_impl::to_rep_t<Number>, RoundingTag>>> {
    };

    template<typename IntegerRep, class IntegerTag, class OutputRoundingTag>
    struct set_rounding<
            _impl::integer<IntegerRep, IntegerTag>, OutputRoundingTag, _impl::enable_if_t<
                    !_impl::is_rounding_tag<IntegerTag>::value>>
            : _impl::type_identity<_impl::integer<set_rounding_t<IntegerRep, OutputRoundingTag>, IntegerTag>> {
    };

    template<typename IntegerRep, class IntegerTag, class OutputRoundingTag>
    struct set_rounding<
            _impl::integer<IntegerRep, IntegerTag>, OutputRoundingTag,
            _impl::enable_if_t<_impl::is_rounding_tag<IntegerTag>::value>>
            : _impl::type_identity<_impl::integer<IntegerRep, OutputRoundingTag>> {
    };

    template<int Digits, class Rep, class RoundingTag>
    struct scale<Digits, 2, _impl::integer<Rep, RoundingTag>,
            _impl::enable_if_t<Digits<0&&_impl::is_rounding_tag<RoundingTag>::value>>
            : _impl::default_scale<Digits, 2, _impl::integer<Rep, RoundingTag>> {
    };

    template<int Digits, int Radix, class Rep, class RoundingTag>
    struct scale<Digits, Radix, _impl::integer<Rep, RoundingTag>,
            _impl::enable_if_t<0<=Digits&&_impl::is_rounding_tag<RoundingTag>::value>> {
        CNL_NODISCARD constexpr auto operator()(_impl::integer<Rep, RoundingTag> const& s) const
        -> decltype(_impl::from_rep<_impl::integer<Rep, RoundingTag>>(
                scale<Digits, Radix, Rep>{}(_impl::to_rep(s))))
        {
            return _impl::from_rep<_impl::integer<Rep, RoundingTag>>(
                    scale<Digits, Radix, Rep>{}(_impl::to_rep(s)));
        }
    };
}

#endif  // CNL_ROUNDING_INTEGER_H
