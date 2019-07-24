
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_INTEGER_FROM_REP_H)
#define CNL_IMPL_INTEGER_FROM_REP_H

#include "is_same_tag_family.h"
#include "type.h"
#include "../num_traits/from_rep.h"
#include "../type_traits/enable_if.h"

#include <type_traits>
#include <utility>

/// compositional numeric library
namespace cnl {
    // when an _impl::integer wraps a non-_impl::integer
    template<typename IntegerRep, class IntegerTag, typename Rep>
    struct from_rep<
            _impl::integer<IntegerRep, IntegerTag>, Rep,
            _impl::enable_if_t<!_impl::is_integer<Rep>::value>> {
        CNL_NODISCARD constexpr auto operator()(Rep const& rep) const
        -> _impl::integer<Rep, IntegerTag>
        {
            return _impl::integer<Rep, IntegerTag>{rep};
        }
    };

    // when one _impl::integer wraps a dissimilar _impl::integer
    template<typename ArchetypeRep, class ArchetypeTag, typename RepRep, class RepTag>
    struct from_rep<
            _impl::integer<ArchetypeRep, ArchetypeTag>,
            _impl::integer<RepRep, RepTag>,
            _impl::enable_if_t<!_impl::can_convert_tag_family<ArchetypeTag, RepTag>::value>> {
        CNL_NODISCARD constexpr auto operator()(_impl::integer<RepRep, RepTag> const& rep) const
        -> _impl::integer<_impl::integer<RepRep, RepTag>, ArchetypeTag>
        {
            return _impl::integer<_impl::integer<RepRep, RepTag>, ArchetypeTag>{rep};
        }
    };

    // when one _impl::integer is converted to a similar _impl::integer
    template<typename ArchetypeRep, class ArchetypeTag, typename RepRep, class RepTag>
    struct from_rep<
            _impl::integer<ArchetypeRep, ArchetypeTag>,
            _impl::integer<RepRep, RepTag>,
            _impl::enable_if_t<_impl::can_convert_tag_family<ArchetypeTag, RepTag>::value>> {
        CNL_NODISCARD constexpr auto operator()(_impl::integer<RepRep, RepTag> const& rep) const
        -> _impl::integer<RepRep, RepTag>
        {
            return _impl::integer<RepRep, RepTag>(rep);
        }
    };
}

#endif  // CNL_IMPL_INTEGER_FROM_REP_H
