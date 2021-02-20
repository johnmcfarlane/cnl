
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_DUPLEX_INTEGER_FORWARD_DECLARATION_H)
#define CNL_IMPL_DUPLEX_INTEGER_FORWARD_DECLARATION_H

/// compositional numeric library
namespace cnl {
    namespace _impl {
        template<typename Upper, typename Lower>
        class duplex_integer;

        template<typename T>
        struct duplex_integer_upper : std::type_identity<T> {
        };

        template<typename Upper, typename Lower>
        struct duplex_integer_upper<duplex_integer<Upper, Lower>>
            : duplex_integer_upper<Upper> {
        };

        template<typename T>
        using duplex_integer_upper_t = typename duplex_integer_upper<T>::type;
    }
}

#endif  // CNL_IMPL_DUPLEX_INTEGER_FORWARD_DECLARATION_H
