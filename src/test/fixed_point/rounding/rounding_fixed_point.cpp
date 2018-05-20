
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/fixed_point.h>
#include <cnl/rounding_integer.h>

namespace {
    using cnl::_impl::identical;
    using cnl::rounding_integer;

    template<class Rep = int, int Exponent = 0, class RoundingTag = cnl::nearest_rounding_tag>
    using rounding_fixed_point = cnl::fixed_point<cnl::rounding_integer<Rep, RoundingTag>, Exponent>;

    namespace test_numeric_limits {
        static_assert(cnl::numeric_limits<rounding_integer<>>::is_specialized,
                "cnl::numeric_limits<rounding_integer<>> test failed");
        static_assert(cnl::numeric_limits<rounding_integer<>>::is_integer,
                "cnl::numeric_limits<rounding_integer<>> test failed");
    }

    namespace test_ctor {
        static_assert(identical(
                to_rep(rounding_fixed_point<>(-8)),
                rounding_integer<>(-8)), "rounding_fixed_point ctor test failed");
        static_assert(rounding_fixed_point<>(0)==cnl::from_rep<rounding_fixed_point<>>{}(0),
                "rounding_fixed_point ctor test failed");
        static_assert(identical(
                to_rep(rounding_fixed_point<std::uint16_t, -8>(15.125)),
                rounding_integer<std::uint16_t>{3872}), "rounding_fixed_point ctor test failed");

        static_assert(identical(
                to_rep(rounding_fixed_point<std::int32_t, -1>(0.249f)),
                rounding_integer<std::int32_t>{0}), "rounding_fixed_point ctor test failed");
        static_assert(identical(
                to_rep(rounding_fixed_point<std::int32_t, -1>(0.25f)),
                rounding_integer<std::int32_t>{1}), "rounding_fixed_point ctor test failed");
    }

    namespace test_conversion {
        constexpr auto a = rounding_fixed_point<int, -4>{.4375};
        constexpr auto b = rounding_fixed_point<int, -1>{a};
        static_assert(identical(
                rounding_fixed_point<int, -1>{.5},
                b), "rounding_fixed_point conversion test failed");
    }
}
