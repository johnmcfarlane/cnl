
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#if !defined(SG14_PRECISE_INTEGER_H)
#define SG14_PRECISE_INTEGER_H 1

#if !defined(SG14_GODBOLT_ORG)

#include <sg14/bits/number_base.h>

#endif

#include <cstdint>
#include <limits>

namespace sg14 {

    template<class Rep>
    struct closest_rounding_policy {
        template<class T>
        static constexpr Rep convert(const T& from)
        {
            return static_cast<Rep>(std::intmax_t(from+((from>=0) ? .5 : -.5)));
        }
    };

    template<class Rep = int, class RoundingPolicy = closest_rounding_policy<Rep>>
    class precise_integer : public _impl::number_base<precise_integer<Rep, RoundingPolicy>, Rep> {
        using super = _impl::number_base<precise_integer<Rep, RoundingPolicy>, Rep>;
    public:
        using rounding = RoundingPolicy;

        constexpr precise_integer() = default;

        template<class T, typename std::enable_if<std::numeric_limits<T>::is_integer, int>::type Dummy = 0>
        constexpr precise_integer(const T& v)
                : super(v) { }

        template<class T, typename std::enable_if<!std::numeric_limits<T>::is_integer, int>::type Dummy = 0>
        constexpr precise_integer(const T& v)
                : super(rounding::convert(v)) { }
    };

    namespace _precise_integer_impl {
        ////////////////////////////////////////////////////////////////////////////////
        // comparison operators

        template<class T>
        struct is_precise_integer : std::false_type {
        };

        template<class Rep, class RoundingPolicy>
        struct is_precise_integer<precise_integer<Rep, RoundingPolicy>> : std::true_type {
        };
    }

    ////////////////////////////////////////////////////////////////////////////////
    // comparison operators

    template<
            class Lhs,
            class RhsRep, class RhsRoundingPolicy,
            typename std::enable_if<!_precise_integer_impl::is_precise_integer<Lhs>::value, int>::type = 0>
    constexpr auto operator==(const Lhs& lhs, const precise_integer<RhsRep, RhsRoundingPolicy>& rhs)
    -> decltype(lhs==to_rep(rhs))
    {
        return lhs==to_rep(rhs);
    };

    template<
            class LhsRep, class LhsRoundingPolicy,
            class Rhs,
            typename std::enable_if<!_precise_integer_impl::is_precise_integer<Rhs>::value, int>::type = 0>
    constexpr auto operator==(const precise_integer<LhsRep, LhsRoundingPolicy>& lhs, const Rhs& rhs)
    -> decltype(to_rep(lhs)==rhs)
    {
        return to_rep(lhs)==rhs;
    };
}

#endif
