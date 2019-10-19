
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_ROUNDING_IS_ROUNDING_TAG_H)
#define CNL_IMPL_ROUNDING_IS_ROUNDING_TAG_H

#include <type_traits>

/// compositional numeric library
namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::is_rounding_tag

        template<typename Tag>
        struct is_rounding_tag : std::false_type {
        };
    }
}

#endif  // CNL_IMPL_ROUNDING_IS_ROUNDING_TAG_H
