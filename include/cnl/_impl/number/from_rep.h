
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_FROM_REP_H)
#define CNL_IMPL_NUMBER_FROM_REP_H

#include "../num_traits/from_rep.h"
#include "../num_traits/tag.h"
#include "../operators/is_same_tag_family.h"
#include "../type_traits/enable_if.h"
#include "definition.h"

#include <type_traits>
#include <utility>

/// compositional numeric library
namespace cnl {
    // when an _impl::number wraps a non-_impl::number
    template<typename NumberRep, class NumberTag, typename Rep>
    struct from_rep<
            _impl::number<NumberRep, NumberTag>, Rep,
            _impl::enable_if_t<_impl::is_homogeneous_operator_tag<NumberTag>::value && !_impl::is_number<Rep>::value>> {
        CNL_NODISCARD constexpr auto operator()(Rep const& rep) const
        -> _impl::number<Rep, NumberTag>
        {
            return rep;
        }
    };

    // when one _impl::number wraps a dissimilar _impl::number
    template<typename ArchetypeRep, class ArchetypeTag, typename Rep>
    struct from_rep<
            _impl::number<ArchetypeRep, ArchetypeTag>, Rep,
            _impl::enable_if_t<!_impl::can_convert_tag_family<ArchetypeTag, _impl::tag_t<Rep>>::value
            &&_impl::is_number<Rep>::value>> {
        CNL_NODISCARD constexpr auto operator()(Rep const& rep) const
        -> _impl::number<Rep, ArchetypeTag>
        {
            return _impl::number<Rep, ArchetypeTag>{rep};
        }
    };

    // when one _impl::number is converted to a similar _impl::number
    template<class Number, class Rep>
    struct from_rep<
            Number, Rep,
            _impl::enable_if_t<_impl::is_number<Number>::value
                    &&_impl::is_number<Rep>::value
                    &&_impl::can_convert_tag_family<_impl::tag_t<Number>, _impl::tag_t<Rep>>::value>> {
        CNL_NODISCARD constexpr auto operator()(Rep const& rep) const
        -> Rep
        {
            return Rep(rep);
        }
    };
}

#endif  // CNL_IMPL_NUMBER_FROM_REP_H
