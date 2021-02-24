
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/elastic_integer.h>
#include <cnl/wide_integer.h>

#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

namespace {
    using cnl::_impl::assert_same;
    using cnl::_impl::identical;

    template<int Digits = cnl::digits<int>, typename Narrowest = int>
    using wide_elastic_integer = cnl::elastic_integer<
            Digits, cnl::wide_integer<cnl::digits<Narrowest>, Narrowest>>;

    namespace default_parameters {
        using cnl::_impl::rep_of_t;
        static_assert(
                assert_same<int, rep_of_t<rep_of_t<wide_elastic_integer<>>>>::value,
                "wide_elastic_integer parameter default test failed");
        static_assert(
                assert_same<
                        rep_of_t<cnl::elastic_integer<>>, rep_of_t<rep_of_t<wide_elastic_integer<>>>>::value,
                "wide_elastic_integer parameter default test failed");
    }

    namespace test_conversion {
        static_assert(identical(wide_elastic_integer<2>{-1}, wide_elastic_integer<2>{-1.500}));
        static_assert(identical(wide_elastic_integer<2>{-1}, wide_elastic_integer<2>{-1.499}));
        static_assert(identical(wide_elastic_integer<2>{-0}, wide_elastic_integer<2>{-.500}));
        static_assert(identical(wide_elastic_integer<2>{0}, wide_elastic_integer<2>{.499}));
        static_assert(identical(wide_elastic_integer<2>{0}, wide_elastic_integer<2>{.500}));
        static_assert(identical(wide_elastic_integer<2>{1}, wide_elastic_integer<2>{1.499}));
        static_assert(identical(wide_elastic_integer<2>{1}, wide_elastic_integer<2>{1.500}));
        static_assert(identical(wide_elastic_integer<2>{2}, wide_elastic_integer<2>{2.499}));
        static_assert(identical(wide_elastic_integer<2>{2}, wide_elastic_integer<2>{2.500}));
    }

    namespace test_division {
        static_assert(
                identical(
                        wide_elastic_integer<2>{3 / 4},
                        wide_elastic_integer<2>{3} / wide_elastic_integer<3>{4}));
        static_assert(identical(wide_elastic_integer<31>{0}, -9 / wide_elastic_integer<4>{10}));
        static_assert(identical(wide_elastic_integer<2>{0}, wide_elastic_integer<2>{-2} / 3));
        static_assert(identical(wide_elastic_integer<2>{0}, wide_elastic_integer<2>{1} / -3));
    }

    namespace test_multiply {
        static_assert(
                identical(
                        wide_elastic_integer<6>{7} * wide_elastic_integer<13>{321},
                        wide_elastic_integer<19>{2247}));
    }

    namespace test_shift_right {
        static_assert(
                identical(
                        wide_elastic_integer<1>{1},
                        wide_elastic_integer<3>{7} >> cnl::constant<2>{}));
        static_assert(identical(wide_elastic_integer<3>{1}, wide_elastic_integer<3>{7} >> 2));
        static_assert(
                identical(
                        wide_elastic_integer<4>{1},
                        cnl::custom_operator<
                                cnl::_impl::shift_right_op,
                                cnl::op_value<wide_elastic_integer<4>>,
                                cnl::op_value<cnl::elastic_integer<2>>>{}(
                                wide_elastic_integer<4>{12}, cnl::elastic_integer<2>{3})));
        static_assert(
                identical(
                        wide_elastic_integer<4>{1},
                        wide_elastic_integer<4>{12} >> cnl::elastic_integer<2>{3}));
    }

    TEST(wide_elastic_integer, pre_increment)  // NOLINT
    {
        auto a = wide_elastic_integer<3>{6};
        auto& b = ++a;
        static_assert(
                assert_same<decltype(b), wide_elastic_integer<3>&>::value,
                "wide_elastic_integer pre-increment return value");
        ASSERT_EQ(&b, &a) << "wide_elastic_integer pre-increment return address";
        ASSERT_EQ(7, b) << "wide_elastic_integer pre-increment";
    }

    TEST(wide_elastic_integer, pre_decrement)  // NOLINT
    {
        auto a = wide_elastic_integer<3>{-6};
        auto& b = --a;
        static_assert(
                assert_same<decltype(b), wide_elastic_integer<3>&>::value,
                "wide_elastic_integer pre-increment return value");
        ASSERT_EQ(&b, &a) << "wide_elastic_integer pre-increment return address";
        ASSERT_EQ(-7, b) << "wide_elastic_integer pre-increment";
    }

    TEST(wide_elastic_integer, post_increment)  // NOLINT
    {
        auto a = wide_elastic_integer<3>{6};
        auto const& b = a++;
        static_assert(
                assert_same<decltype(b), wide_elastic_integer<3> const&>::value,
                "wide_elastic_integer pre-increment return value");
        ASSERT_NE(&b, &a) << "wide_elastic_integer pre-increment return address";
        ASSERT_EQ(7, a) << "wide_elastic_integer pre-increment";
        ASSERT_EQ(6, b) << "wide_elastic_integer pre-increment";
    }

    TEST(wide_elastic_integer, post_decrement)  // NOLINT
    {
        auto a = wide_elastic_integer<3>{-6};
        auto const& b = a--;
        static_assert(
                assert_same<decltype(b), wide_elastic_integer<3> const&>::value,
                "wide_elastic_integer pre-increment return value");
        ASSERT_NE(&b, &a) << "wide_elastic_integer pre-increment return address";
        ASSERT_EQ(-7, a) << "wide_elastic_integer pre-increment";
        ASSERT_EQ(-6, b) << "wide_elastic_integer pre-increment";
    }
}
