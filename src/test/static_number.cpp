
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/static_number.h>

#include <gtest/gtest.h>

#include <array>
#include <string>

namespace {
    using cnl::_impl::identical;
    using cnl::_impl::assert_same;

    namespace default_parameters {
        static_assert(
                std::is_same<cnl::static_number<1>::rep::rep::rep::rep, int>::value,
                "cnl::rounding_integer parameter default test failed");
    }

    namespace test_rounding_t {
        static_assert(
                assert_same<
                        cnl::_impl::nearest_rounding_tag,
                        cnl::_impl::rounding_t<cnl::static_number<1>>
                >::value,
                "cnl::_impl::rounding_t<cnl::static_number<>> test failed");
    }

    namespace test_set_rounding_t {
        static_assert(
                assert_same<
                        cnl::static_number<1, 0, cnl::native_rounding_tag>,
                        cnl::_impl::set_rounding_t<
                                cnl::static_number<1>,
                                cnl::native_rounding_tag>
                >::value,
                "cnl::_impl::set_rounding_t<cnl::static_number<>> test failed");
    }

    namespace test_make_static_number {
        using namespace cnl::literals;
        static_assert(identical(cnl::make_static_number(cnl::int16{7}), cnl::static_number<15>{7}), "");
        static_assert(identical(cnl::make_static_number(444_c), cnl::static_number<7, 2>{444}), "");
    }

    TEST(static_number, to_chars)
    {
        auto const num_chars = 12;
        auto buffer = std::array<char, num_chars+1>{};
        auto chars_first = buffer.data();
        auto chars_last = chars_first+num_chars;
        auto result = cnl::to_chars(
                chars_first,
                chars_last,
                cnl::static_number<24, -8>{-45678.765625});
        ASSERT_EQ(std::errc{}, result.ec);
        ASSERT_EQ(chars_last, result.ptr);
        *chars_last = '\0';

        ASSERT_EQ(std::string{"-45678.76562"}, chars_first);
    }

#if defined(CNL_INT128_ENABLED)
    TEST(static_number, to_chars_wider)
    {
        auto const num_chars = 12;
        auto buffer = std::array<char, num_chars+1>{};
        auto chars_first = buffer.data();
        auto chars_last = chars_first+num_chars;
        auto result = cnl::to_chars(
                chars_first,
                chars_last,
                cnl::static_number<28, -8>{937016.765625});
        ASSERT_EQ(std::errc{}, result.ec);
        ASSERT_EQ(chars_last, result.ptr);
        *chars_last = '\0';

        ASSERT_EQ(std::string{"937016.76562"}, chars_first);
    }
#endif

    namespace test_multiply {
        static_assert(identical(
                cnl::static_number<6>{7}*cnl::static_number<13>{321},
                cnl::static_number<19>{2247}), "");
    }

    namespace test_conversion1 {
        using namespace cnl;
        constexpr auto a = cnl::static_number<8, -4>{.4375};
        constexpr auto b = cnl::static_number<5, -1>{a};
        static_assert(identical(cnl::static_number<5, -1>{.5}, b), "");
    }

#if !defined(CNL_UNREACHABLE_UB_ENABLED)
    TEST(static_number, most_negative_number) {
        static_assert(cnl::static_number<1>{1}, "in-range boundary test");
        static_assert(cnl::static_number<1>{-1}, "in-range boundary test");
        ASSERT_DEATH(cnl::static_number<1>{-2}, "negative overflow");
    }
#endif

    TEST(static_number, pre_increment) {
        auto a = cnl::static_number<4, -2>{2.75};
        auto& b = ++ a;
        static_assert(
                std::is_same<decltype(b), cnl::static_number<4, -2>&>::value,
                "static_number pre-increment return value");
        ASSERT_EQ(&b, &a) << "static_number pre-increment return address";
        ASSERT_EQ(3.75, b) << "static_number pre-increment";
    }

    TEST(static_number, pre_decrement) {
        auto a = cnl::static_number<4, -2>{-2.75};
        auto& b = -- a;
        static_assert(
                std::is_same<decltype(b), cnl::static_number<4, -2>&>::value,
                "static_number pre-increment return value");
        ASSERT_EQ(&b, &a) << "static_number pre-increment return address";
        ASSERT_EQ(-3.75, b) << "static_number pre-increment";
    }

    TEST(static_number, post_increment) {
        auto a = cnl::static_number<4, -2>{2.75};
        auto const& b = a ++;
        static_assert(
                std::is_same<decltype(b), cnl::static_number<4, -2> const&>::value,
                "static_number pre-increment return value");
        ASSERT_NE(&b, &a) << "static_number pre-increment return address";
        ASSERT_EQ(3.75, a) << "static_number pre-increment";
        ASSERT_EQ(2.75, b) << "static_number pre-increment";
    }

    TEST(static_number, post_decrement) {
        auto a = cnl::static_number<4, -2>{-2.75};
        auto const& b = a --;
        static_assert(
                std::is_same<decltype(b), cnl::static_number<4, -2> const&>::value,
                "static_number pre-increment return value");
        ASSERT_NE(&b, &a) << "static_number pre-increment return address";
        ASSERT_EQ(-3.75, a) << "static_number pre-increment";
        ASSERT_EQ(-2.75, b) << "static_number pre-increment";
    }

#if !defined(CNL_UNREACHABLE_UB_ENABLED)
    TEST(static_number, pre_increment_overflow) {
        auto a = cnl::static_number<4, -2>{3.0};
        ASSERT_DEATH(++ a, "positive overflow");
    }

    TEST(static_number, pre_decrement_overflow) {
        auto a = cnl::static_number<4, -2>{-3.0};
        ASSERT_DEATH(-- a, "negative overflow");
    }

    TEST(static_number, post_increment_overflow) {
        auto a = cnl::static_number<4, -2>{3.0};
        ASSERT_DEATH(a ++, "positive overflow");
    }

    TEST(static_number, post_decrement_overflow) {
        auto a = cnl::static_number<4, -2>{-3.0};
        ASSERT_DEATH(a --, "negative overflow");
    }
#endif
}
