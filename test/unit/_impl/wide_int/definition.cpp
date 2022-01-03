
//          Copyright John McFarlane 2019.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wide_integer/definition.h>

#include <cnl/_impl/wide_integer/definition.h>

#include <cnl/_impl/type_traits/identical.h>
#include <cnl/cstdint.h>
#include <cnl/wide_integer.h>

#include <gtest/gtest.h>

#include <type_traits>

using cnl::_impl::identical;

namespace {
    namespace test_parameters {
        static_assert(
                std::is_same_v<cnl::wide_integer<31>, cnl::wide_integer<>>);
    }

    namespace test_ctor {
        static_assert(
                identical(cnl::wide_integer<2, int>{}, cnl::wide_integer<2, int>{0}),
                "constructor taking wide_integer");
    }

    TEST(wide_integer, post_decrement)  // NOLINT
    {
        auto a = cnl::wide_integer<3>{-6};
        auto const& b = a--;
        static_assert(std::is_same_v<decltype(b), cnl::wide_integer<3> const&>);
        ASSERT_NE(&b, &a) << "cnl::wide_integer pre-increment return address";
        ASSERT_EQ(-7, a) << "cnl::wide_integer pre-increment";
        ASSERT_EQ(-6, b) << "cnl::wide_integer pre-increment";
    }

    namespace test_max_digits {
        static_assert(identical(
                0x7fffffff,
                cnl::_impl::max_digits<cnl::wide_integer<0>>));
    }

    namespace test_add {
        static_assert(
                identical(
                        cnl::wide_integer<31>{2},
                        cnl::custom_operator<
                                cnl::_impl::add_op,
                                cnl::op_value<cnl::wide_integer<31>>,
                                cnl::op_value<cnl::wide_integer<31>>>{}(
                                cnl::wide_integer<31>{1}, cnl::wide_integer<31>{1})));
        static_assert(
                identical(
                        cnl::wide_integer<31>{2},
                        cnl::wide_integer<31>{1} + cnl::wide_integer<31>{1}));
    }

    namespace test_shift_left {
        static_assert(
                identical(
                        cnl::wide_integer<31>{2},
                        cnl::custom_operator<
                                cnl::_impl::add_op,
                                cnl::op_value<cnl::wide_integer<31>>,
                                cnl::op_value<cnl::wide_integer<31>>>{}(
                                cnl::wide_integer<31>{1}, cnl::wide_integer<31>{1})));
        static_assert(
                identical(
                        cnl::wide_integer<31>{2},
                        cnl::wide_integer<31>{1} + cnl::wide_integer<31>{1}));
    }
}
