
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/wide_integer.h>

#include <cnl/limits.h>
#include <cnl/wide_integer.h>

#include <cnl/cstdint.h>
#include <cnl/_impl/type_traits/assert_same.h>
#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

using cnl::_impl::assert_same;
using cnl::_impl::identical;

namespace {
    namespace test_parameters {
        static_assert(
                assert_same<cnl::wide_integer<31>, cnl::wide_integer<>>::value,
                "default parameters of cnl::wide_integer");
    }

    namespace test_ctor {
        static_assert(
                identical(cnl::wide_integer<2, int>{}, cnl::wide_integer<2, int>{0}),
                "constructor taking wide_integer");
    }

    TEST(wide_integer, post_decrement) {
        auto a = cnl::wide_integer<3>{-6};
        auto const& b = a --;
        static_assert(
                assert_same<decltype(b), cnl::wide_integer<3> const&>::value,
                "cnl::wide_integer pre-increment return value");
        ASSERT_NE(&b, &a) << "cnl::wide_integer pre-increment return address";
        ASSERT_EQ(-7, a) << "cnl::wide_integer pre-increment";
        ASSERT_EQ(-6, b) << "cnl::wide_integer pre-increment";
    }
}
