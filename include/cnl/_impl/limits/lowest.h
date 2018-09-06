
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(CNL_IMPL_LIMITS_LOWEST_H)
#define CNL_IMPL_LIMITS_LOWEST_H

/// compositional numeric library
namespace cnl {
    namespace _impl {
        // helper for numeric_limits<>::lowest()
        template<class Rep, bool IsSigned>
        struct lowest;

        template<class Rep>
        struct lowest<Rep, true> {
            constexpr Rep operator()(Rep const& max) const noexcept
            {
                return static_cast<Rep>(-max);
            }
        };

        template<class Rep>
        struct lowest<Rep, false> {
            constexpr Rep operator()(Rep const&) const noexcept
            {
                return 0;
            }
        };
    }
}

#endif  // CNL_IMPL_LIMITS_LOWEST_H
