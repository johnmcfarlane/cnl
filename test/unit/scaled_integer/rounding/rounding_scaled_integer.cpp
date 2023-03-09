
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/rounding_integer.h>
#include <cnl/scaled_integer.h>

#include <cinttypes>

#define TEST_NATIVE_INTEGER
#define TEST_LABEL rounding_integer_  // NOLINT(cppcoreguidelines-macro-usage)

////////////////////////////////////////////////////////////////////////////////
// integer types used as scaled_integer Rep type

using test_int = cnl::rounding_integer<int, cnl::native_rounding_tag>;

////////////////////////////////////////////////////////////////////////////////
// perform scaled_integer tests with this type of scaled_integer specialization

#include "../scaled_integer_common.h"

namespace {
    using cnl::_impl::identical;
    using cnl::rounding_integer;

    template<class Rep = int, int Exponent = 0, class RoundingTag = cnl::nearest_rounding_tag>
    using rounding_scaled_integer = cnl::scaled_integer<cnl::rounding_integer<Rep, RoundingTag>, cnl::power<Exponent>>;

    namespace test_numeric_limits {
        static_assert(cnl::numeric_limits<rounding_integer<>>::is_specialized,
                "cnl::numeric_limits<rounding_integer<>> test failed");
        static_assert(cnl::numeric_limits<rounding_integer<>>::is_integer,
                "cnl::numeric_limits<rounding_integer<>> test failed");
    }

    namespace test_ctor {
        static_assert(identical(
                to_rep(rounding_scaled_integer<>(-8)),
                rounding_integer<>(-8)), "rounding_scaled_integer ctor test failed");
        static_assert(rounding_scaled_integer<>(0)==cnl::_impl::from_rep<rounding_scaled_integer<>>(0),
                "rounding_scaled_integer ctor test failed");
        static_assert(identical(
                to_rep(rounding_scaled_integer<std::uint16_t, -8>(15.125)),
                rounding_integer<std::uint16_t>{3872}), "rounding_scaled_integer ctor test failed");

        static_assert(identical(
                to_rep(rounding_scaled_integer<std::int32_t, -1>(0.249F)),
                rounding_integer<std::int32_t>{0}), "rounding_scaled_integer ctor test failed");
        static_assert(identical(
                to_rep(rounding_scaled_integer<std::int32_t, -1>(0.25F)),
                rounding_integer<std::int32_t>{1}), "rounding_scaled_integer ctor test failed");
    }

    namespace test_conversion {
        constexpr auto a = rounding_scaled_integer<int, -4>{.4375};
        constexpr auto b = rounding_scaled_integer<int, -1>{a};
        static_assert(identical(
                rounding_scaled_integer<int, -1>{.5},
                b), "rounding_scaled_integer conversion test failed");
    }

    namespace test_conversion_nearest {
        using source_type = rounding_scaled_integer<int, -2>;
        using dest_type = rounding_scaled_integer<int, 0, cnl::nearest_rounding_tag>;

        static_assert(identical(dest_type{-1}, dest_type{source_type{-.75}}), "");
        static_assert(identical(dest_type{-1}, dest_type{source_type{-.5}}), "");
        static_assert(identical(dest_type{0}, dest_type{source_type{.25}}), "");
        static_assert(identical(dest_type{1}, dest_type{source_type{.5}}), "");
    }

    namespace test_conversion_native {
        using source_type = rounding_scaled_integer<int, -2>;
        using dest_type = rounding_scaled_integer<int, 0, cnl::native_rounding_tag>;

        static_assert(identical(dest_type{0}, dest_type{source_type{-.25}}), "");
        static_assert(identical(dest_type{0}, dest_type{source_type{0}}), "");
        static_assert(identical(dest_type{0}, dest_type{source_type{.75}}), "");
        static_assert(identical(dest_type{1}, dest_type{source_type{1.}}), "");
    }

    TEST(rounding_scaled_integer, 991)
    {
        using DecX = cnl::scaled_integer<cnl::rounding_integer<long long>, cnl::power<-1, 100>>;
        DecX num1 = 10.554;
        DecX num2 = 10.545;
        std::cout<<num1<<" "<<num2<<std::endl;
    }

    TEST(rounding_scaled_integer, 991AssignTest)
    {
        using DecX_2 = cnl::scaled_integer<cnl::rounding_integer<long long>, cnl::power<-2, 10>>;
        using DecX_4 = cnl::scaled_integer<cnl::rounding_integer<long long>, cnl::power<-4, 10>>;
        
        DecX_2 n1 = 10.0151;
        DecX_2 n2 = 10.0249;
        EXPECT_EQ(n1, n2); // 10.02

        int64_t xn = (int64_t)n1;
        double  xm = (double)n2;
        EXPECT_EQ(xn, 10);
        EXPECT_DOUBLE_EQ(xm, 10.02);
        
        static_assert(std::is_same_v<decltype(n1*n2), DecX_4>);
        EXPECT_EQ(n1*n2, DecX_4{100.4004});
        EXPECT_DOUBLE_EQ((double)(n1*n2), 100.4004);

        n1 += 0.6;
        EXPECT_EQ((int)n1, 11);

        n1 += 0.38;
        EXPECT_EQ(n1, 11);

        xn += n1;
        xm -= n1;
    }

    TEST(rounding_scaled_integer, 991CmpTest)
    {
        using DecX_2 = cnl::scaled_integer<cnl::rounding_integer<long long>, cnl::power<-2, 10>>;
        
        DecX_2 n1;
        DecX_2 n2 = 10.019;
        DecX_2 n3{20.029};
        EXPECT_DOUBLE_EQ((double)n2, 10.02);
        EXPECT_DOUBLE_EQ((double)n3, 20.03);

        n1 = 3.999; //4.00 instead
        n3 = DecX_2(8);
        EXPECT_NE(n1, 3.9899999999999999999999999);
        EXPECT_EQ(n1, 4.0000000000000000000000001);
        EXPECT_EQ(n3, 8);
    }
}
