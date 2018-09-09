
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef CNL_IMPL_NATIVE_TAG_H
#define CNL_IMPL_NATIVE_TAG_H

#include "type_traits/make_unsigned.h"

#include "operators.h"

////////////////////////////////////////////////////////////////////////////////
// tag error message

template<typename Type>
struct CNL_ERROR___cannot_use {
    // It you see this type in an error message then somewhere,
    // you probably passed a non-tag type as a tag parameter.
    struct as_a_tag;
};

/// compositional numeric library
namespace cnl {
    namespace _impl {

        // match the behavior of fundamental arithmetic types
        struct native_tag {};
    }

    ////////////////////////////////////////////////////////////////////////////////
    // cnl::convert

    template<class Tag, typename Result, typename Input, typename Enable = void>
    struct convert : public CNL_ERROR___cannot_use<Tag>::as_a_tag {
    };

    template<typename Result, typename Input>
    struct convert<_impl::native_tag, Result, Input> {
        constexpr Result operator()(Input const& rhs) const
        {
            return static_cast<Result>(rhs);
        }
    };

    namespace _impl {
        template<class Tag, typename Result, typename Input>
        constexpr auto convert(Input const& from)
        -> decltype(cnl::convert<Tag, Result, Input>{}(from))
        {
            return cnl::convert<Tag, Result, Input>{}(from);
        }

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::tagged_binary_operator

        template<class Tag, class Operator>
        struct tagged_binary_operator : public CNL_ERROR___cannot_use<Tag>::as_a_tag {
        };

        template<class Operator>
        struct tagged_binary_operator<native_tag, Operator> : Operator {};

        template<>
        struct tagged_binary_operator<native_tag, shift_left_op> {
            template<class Lhs, class Rhs>
            constexpr auto operator()(Lhs const& lhs, Rhs const& rhs) const
            -> decltype(lhs<<rhs)
            {
                using result_type = decltype(lhs<<rhs);
                return static_cast<result_type>(static_cast<cnl::make_unsigned_t<result_type>>(lhs)<<rhs);
            }
        };
    }
}

#endif  // CNL_IMPL_NATIVE_TAG_H
