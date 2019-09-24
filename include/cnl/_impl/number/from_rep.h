
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_FROM_REP_H)
#define CNL_IMPL_NUMBER_FROM_REP_H

#include "definition.h"
#include "../num_traits/from_rep.h"
#include "cnl/_impl/operators/homogeneous_operator_tag_base.h"
#include "../operators/is_same_tag_family.h"
#include "../type_traits/enable_if.h"

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
            return _impl::number<Rep, NumberTag>{rep};
        }
    };

    // when one _impl::number wraps a dissimilar _impl::number
    template<typename ArchetypeRep, class ArchetypeTag, typename RepRep, class RepTag>
    struct from_rep<
            _impl::number<ArchetypeRep, ArchetypeTag>,
            _impl::number<RepRep, RepTag>,
            _impl::enable_if_t<!_impl::can_convert_tag_family<ArchetypeTag, RepTag>::value>> {
        CNL_NODISCARD constexpr auto operator()(_impl::number<RepRep, RepTag> const& rep) const
        -> _impl::number<_impl::number<RepRep, RepTag>, ArchetypeTag>
        {
            return _impl::number<_impl::number<RepRep, RepTag>, ArchetypeTag>{rep};
        }
    };

    // when one _impl::number is converted to a similar _impl::number
    template<typename ArchetypeRep, class ArchetypeTag, typename RepRep, class RepTag>
    struct from_rep<
            _impl::number<ArchetypeRep, ArchetypeTag>,
            _impl::number<RepRep, RepTag>,
            _impl::enable_if_t<_impl::can_convert_tag_family<ArchetypeTag, RepTag>::value>> {
        CNL_NODISCARD constexpr auto operator()(_impl::number<RepRep, RepTag> const& rep) const
        -> _impl::number<RepRep, RepTag>
        {
            return _impl::number<RepRep, RepTag>(rep);
        }
    };
}

#endif  // CNL_IMPL_NUMBER_FROM_REP_H
