
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_TYPE_TRAITS_SET_SIGNEDNESS)
#define CNL_IMPL_TYPE_TRAITS_SET_SIGNEDNESS

#include "add_signedness.h"
#include "remove_signedness.h"

namespace cnl {
    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::set_signedness - std::add_signedness with IsSigned parameter

        template<class T, bool IsSigned>
        struct set_signedness;

        template<class T>
        struct set_signedness<T, true> : add_signedness<T> {
        };

        template<class T>
        struct set_signedness<T, false> : remove_signedness<T> {
        };

        template<class T, bool IsSigned>
        using set_signedness_t = typename set_signedness<T, IsSigned>::type;
    }
}

#endif // CNL_IMPL_TYPE_TRAITS_SET_SIGNEDNESS
