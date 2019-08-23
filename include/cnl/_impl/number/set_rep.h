
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUMBER_SET_REP_H)
#define CNL_IMPL_NUMBER_SET_REP_H

#include "../num_traits/set_rep.h"
#include "../type_traits/type_identity.h"

/// compositional numeric library
namespace cnl {
    // when an _impl::number wraps a non-_impl::number
    template<typename NumberRep, typename Rep>
    struct set_rep<_impl::number<NumberRep>, Rep>
        : _impl::type_identity<_impl::number<Rep>> {
    };

    // when one _impl::number wraps a dissimilar _impl::number
    template<typename ArchetypeRep, class ArchetypeTag, typename RepRep, class RepTag>
    struct set_rep<
            _impl::number<ArchetypeRep, ArchetypeTag>,
            _impl::number<RepRep, RepTag>,
            _impl::enable_if_t<!_impl::can_convert_tag_family<ArchetypeTag, RepTag>::value>>
        : _impl::type_identity<_impl::number<_impl::number<RepRep, RepTag>, ArchetypeTag>> {
    };

    // when one _impl::number is converted to a similar _impl::number
    template<typename ArchetypeRep, class ArchetypeTag, typename RepRep, class RepTag>
    struct set_rep<
            _impl::number<ArchetypeRep, ArchetypeTag>,
            _impl::number<RepRep, RepTag>,
            _impl::enable_if_t<_impl::can_convert_tag_family<ArchetypeTag, RepTag>::value>>
        : _impl::type_identity<_impl::number<RepRep, RepTag>> {
    };
}

#endif  // CNL_IMPL_NUMBER_SET_REP_H
