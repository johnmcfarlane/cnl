
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_WRAPPER_SET_REP_H)
#define CNL_IMPL_WRAPPER_SET_REP_H

#include "../num_traits/set_rep.h"
#include "definition.h"

#include <type_traits>

/// compositional numeric library
namespace cnl {
    // when an _impl::wrapper wraps a non-_impl::wrapper
    template<typename NumberRep, typename Rep>
    struct set_rep<_impl::wrapper<NumberRep>, Rep> : std::type_identity<_impl::wrapper<Rep>> {
    };

    // when one _impl::wrapper wraps a dissimilar _impl::wrapper
    template<typename ArchetypeRep, class ArchetypeTag, _impl::wrapped Rep>
    struct set_rep<
            _impl::wrapper<ArchetypeRep, ArchetypeTag>, Rep,
            _impl::enable_if_t<
                    !_impl::can_convert_tag_family<ArchetypeTag, _impl::tag_of_t<Rep>>::value>>
        : std::type_identity<_impl::wrapper<Rep, ArchetypeTag>> {
    };

    // when one _impl::wrapper is converted to a similar _impl::wrapper
    template<typename ArchetypeRep, class ArchetypeTag, _impl::wrapped Rep>
    struct set_rep<
            _impl::wrapper<ArchetypeRep, ArchetypeTag>, Rep,
            _impl::enable_if_t<
                    _impl::can_convert_tag_family<ArchetypeTag, _impl::tag_of_t<Rep>>::value>>
        : std::type_identity<Rep> {
    };
}

#endif  // CNL_IMPL_WRAPPER_SET_REP_H
