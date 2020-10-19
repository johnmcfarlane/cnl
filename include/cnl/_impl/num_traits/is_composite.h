
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_NUM_TRAITS_IS_COMPOSITE)
#define CNL_IMPL_NUM_TRAITS_IS_COMPOSITE

#include <type_traits>

namespace cnl {
    template<class T, class Enable = void>
    struct is_composite : std::false_type {
        static_assert(!std::is_const<T>::value, "T is const");
        static_assert(!std::is_volatile<T>::value, "T is volatile");
    };

    template<class T>
    constexpr auto is_composite_v = is_composite<T>::value;

    namespace _impl {
        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::are_composite

        template<class ... Args>
        struct are_composite;

        template<>
        struct are_composite<> : std::false_type {
        };

        template<class ArgHead, class ... ArgTail>
        struct are_composite<ArgHead, ArgTail...>
                : std::integral_constant<bool, is_composite<typename std::decay<ArgHead>::type>::value || are_composite<ArgTail...>::value> {
        };
    }
}

#endif  // CNL_IMPL_NUM_TRAITS_IS_COMPOSITE
