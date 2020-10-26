
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_ROUNDING_INTEGER_H)
#define CNL_ROUNDING_INTEGER_H

#include "_impl/num_traits/is_composite.h"
#include "_impl/num_traits/rep.h"
#include "_impl/num_traits/rounding.h"
#include "_impl/num_traits/set_rounding.h"
#include "_impl/num_traits/to_rep.h"
#include "_impl/number.h"
#include "_impl/operators/tagged.h"
#include "_impl/rounding.h"
#include "_impl/rounding/convert_operator.h"
#include "_impl/rounding/nearest_rounding_tag.h"
#include "_impl/type_traits/type_identity.h"

#include <ostream>

/// compositional numeric library
namespace cnl {
    /// \brief An integer which detects overflow.
    template<typename Rep = int, class Tag = nearest_rounding_tag>
    using rounding_integer = _impl::number<Rep, Tag>;

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::rounding

    template<class Number>
    struct rounding<
            Number,
            _impl::enable_if_t<_impl::is_number<Number>::value && !_impl::is_rounding_tag<typename Number::tag>::value>>
            : rounding<typename Number::rep> {
    };

    template<typename Rep, class RoundingTag>
    struct rounding<_impl::number<Rep, RoundingTag>, _impl::enable_if_t<_impl::is_rounding_tag<RoundingTag>::value>>
            : _impl::type_identity<RoundingTag> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::_impl::set_rounding

    template<typename Number, class RoundingTag>
    struct set_rounding<Number, RoundingTag, _impl::enable_if_t<
            is_composite<Number>::value && !_impl::is_number<Number>::value>>
            : _impl::type_identity<
                    _impl::set_rep_t<
                            Number,
                            set_rounding_t<_impl::rep_t<Number>, RoundingTag>>> {
    };

    template<int Digits, class Rep, class RoundingTag>
    struct scale<Digits, 2, _impl::number<Rep, RoundingTag>,
            _impl::enable_if_t<Digits < 0 && _impl::is_rounding_tag<RoundingTag>::value>>
            : _impl::default_scale<Digits, 2, _impl::number<Rep, RoundingTag>> {
    };

    template<int Digits, int Radix, class Rep, class RoundingTag>
    struct scale<Digits, Radix, _impl::number<Rep, RoundingTag>,
            _impl::enable_if_t<0 <= Digits && _impl::is_rounding_tag<RoundingTag>::value>> {
        CNL_NODISCARD constexpr auto operator()(_impl::number<Rep, RoundingTag> const& s) const
        {
            return _impl::from_rep<_impl::number<Rep, RoundingTag>>(
                    scale<Digits, Radix, Rep>{}(_impl::to_rep(s)));
        }
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::set_rep<rounding_integer, Rep>

    template<typename NumberRep, class NumberTag, typename Rep>
    struct set_rep<
            _impl::number<NumberRep, NumberTag>, Rep,
            _impl::enable_if_t<_impl::is_rounding_tag<NumberTag>::value
                    && !_impl::is_number<Rep>::value>>
            : _impl::type_identity<_impl::number<Rep, NumberTag>> {
    };

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::set_tag<rounding_integer, Tag>

    template<typename NumberRep, class NumberTag, class Tag>
    struct set_tag<
            _impl::number<NumberRep, NumberTag>, Tag,
            _impl::enable_if_t<_impl::is_rounding_tag<NumberTag>::value>>
            : _impl::type_identity<_impl::number<NumberRep, Tag>> {
    };
}

#endif  // CNL_ROUNDING_INTEGER_H
