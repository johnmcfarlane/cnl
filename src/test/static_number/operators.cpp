
//          Copyright John McFarlane 2015 - 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/static_number.h>
#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

using cnl::_impl::identical;

namespace {
    namespace test_multiply {
        static_assert(identical(
                cnl::static_number<6>{7}*cnl::static_number<13>{321},
                cnl::static_number<19>{2247}), "");
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
