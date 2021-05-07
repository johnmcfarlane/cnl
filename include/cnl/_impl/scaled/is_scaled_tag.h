
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_SCALED_IS_SCALED_TAG_H)
#define CNL_IMPL_SCALED_IS_SCALED_TAG_H

#include <type_traits>

/// compositional numeric library
namespace cnl {
    template<class Tag>
    struct is_scaled_tag : std::false_type {
    };

    template<class Tag>
    inline constexpr auto is_scaled_tag_v = is_scaled_tag<Tag>::value;

    template<class Tag>
    concept scaled_tag = is_scaled_tag_v<Tag>;

    namespace _impl {
        template<typename Number>
        [[nodiscard]] constexpr auto not_scaled_integer(Number const& n)
        {
            return n;
        }

        // template<typename Number>
        // struct exponent : std::integral_constant<int, 0> {
        // };

        // template<typename Number>
        // struct radix : std::integral_constant<int, 0> {
        // };

        template<typename T>
        inline constexpr auto exponent_v = 0;

        template<typename T>
        inline constexpr auto radix_v = 0;
    }

    template<scaled_tag Lhs, scaled_tag Rhs>
    [[nodiscard]] constexpr auto operator<(Lhs const&, Rhs const&)
    {
        static_assert(_impl::radix_v<Lhs> == _impl::radix_v<Rhs>);
        return _impl::exponent_v<Lhs> < _impl::exponent_v<Rhs>;
    }
}

#endif  // CNL_IMPL_SCALED_IS_SCALED_TAG_H
