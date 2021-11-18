
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/rounding_integer.h>
#include <cnl/scaled_integer.h>

#include <cnl/_impl/num_traits/to_rep.h>
#include <cnl/_impl/type_traits/identical.h>

#include <cinttypes>
#include <limits>

#define TEST_NATIVE_INTEGER
#define TEST_LABEL rounding_integer_  // NOLINT(cppcoreguidelines-macro-usage)

////////////////////////////////////////////////////////////////////////////////
// integer types used as scaled_integer Rep type

using test_int = cnl::rounding_integer<int, cnl::native_rounding_tag>;

////////////////////////////////////////////////////////////////////////////////
// perform scaled_integer tests with this type of scaled_integer specialization

#include "../scaled_int_common.h"

namespace {
    using cnl::rounding_integer;
    using cnl::_impl::identical;

    template<class Rep = int, int Exponent = 0, cnl::rounding_tag RoundingTag = cnl::nearest_rounding_tag>
    using rounding_scaled_integer =
            cnl::scaled_integer<cnl::rounding_integer<Rep, RoundingTag>, cnl::power<Exponent>>;

    namespace test_numeric_limits {
        static_assert(
                std::numeric_limits<rounding_integer<>>::is_specialized,
                "std::numeric_limits<rounding_integer<>> test failed");
        static_assert(
                std::numeric_limits<rounding_integer<>>::is_integer,
                "std::numeric_limits<rounding_integer<>> test failed");
    }

    namespace test_ctor {
        static_assert(
                identical(to_rep(rounding_scaled_integer<>(-8)), rounding_integer<>(-8)),
                "rounding_scaled_integer ctor test failed");
        static_assert(
                rounding_scaled_integer<>(0) == cnl::_impl::from_rep<rounding_scaled_integer<>>(0),
                "rounding_scaled_integer ctor test failed");
        static_assert(
                identical(
                        to_rep(rounding_scaled_integer<std::uint16_t, -8>(15.125)),
                        rounding_integer<std::uint16_t>{3872}),
                "rounding_scaled_integer ctor test failed");

        static_assert(
                identical(
                        to_rep(rounding_scaled_integer<std::int32_t, -1>(0.249F)),
                        rounding_integer<std::int32_t>{0}),
                "rounding_scaled_integer ctor test failed");
        static_assert(
                identical(
                        to_rep(rounding_scaled_integer<std::int32_t, -1>(0.25F)),
                        rounding_integer<std::int32_t>{1}),
                "rounding_scaled_integer ctor test failed");
    }

    namespace test_conversion {
        constexpr auto a = rounding_scaled_integer<int, -4>{.4375};
        constexpr auto b = rounding_scaled_integer<int, -1>{a};
        static_assert(
                identical(rounding_scaled_integer<int, -1>{.5}, b),
                "rounding_scaled_integer conversion test failed");
    }

    namespace test_conversion_nearest {
        using source_type = rounding_scaled_integer<int, -2>;
        using dest_type = rounding_scaled_integer<int, 0, cnl::nearest_rounding_tag>;

        static_assert(identical(dest_type{-1}, dest_type{source_type{-.75}}));
        static_assert(identical(dest_type{-1}, dest_type{source_type{-.5}}));
        static_assert(identical(dest_type{0}, dest_type{source_type{.25}}));
        static_assert(identical(dest_type{1}, dest_type{source_type{.5}}));
    }

    namespace test_conversion_native {
        using source_type = rounding_scaled_integer<int, -2>;
        using dest_type = rounding_scaled_integer<int, 0, cnl::native_rounding_tag>;

        static_assert(identical(dest_type{0}, dest_type{source_type{-.25}}));
        static_assert(identical(dest_type{0}, dest_type{source_type{0}}));
        static_assert(identical(dest_type{0}, dest_type{source_type{.75}}));
        static_assert(identical(dest_type{1}, dest_type{source_type{1.}}));
    }

    namespace test_jokes {
        using nearest_tens =
                cnl::scaled_integer<cnl::rounding_integer<>, cnl::power<1, 10>>;

        // https://twitter.com/davechannel/status/1163366347843276800
        constexpr auto expected_cows{20};
        constexpr auto actual_cows{int{nearest_tens{18}}};
        static_assert(identical(expected_cows, actual_cows));

        // https://twitter.com/STDeltaShift/status/1372908408651849732
        constexpr auto expected_sheep{50};
        constexpr auto actual_sheep{int{nearest_tens{48}}};
        static_assert(identical(expected_sheep, actual_sheep));
    }
}
