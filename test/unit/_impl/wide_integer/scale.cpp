
//          Copyright John McFarlane 2018.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file ../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/// \file
/// \brief tests for <cnl/_impl/wide_integer/scale.h>

#include <cnl/_impl/wide_integer/scale.h>

#include <cnl/_impl/num_traits/fixed_width_scale.h>
#include <cnl/wide_integer.h>

#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

using cnl::_impl::identical;

namespace {
    namespace test_power_value {
        static_assert(
                identical(
                        cnl::wide_integer<16, cnl::int8>{256},
                        cnl::_impl::power_value<cnl::wide_integer<16, cnl::uint8>, 8, 2>()),
                "cnl::_impl::power_value<cnl::wide_integer<16>, ...>");
        static_assert(
                identical(
                        cnl::wide_integer<100, cnl::uint8>{1} << 80,
                        cnl::_impl::power_value<cnl::wide_integer<100, cnl::uint8>, 80, 2>()),
                "cnl::_impl::power_value<cnl::wide_integer<16>, ...>");
    }

    namespace test_scale {
        static_assert(
                identical(
                        cnl::wide_integer<16, unsigned>{0x12},
                        cnl::wide_integer<16, unsigned>{0x1234}/cnl::wide_integer<16, unsigned>{0x100}),
                "cnl::_impl::scale test failed");
        static_assert(
                identical(
                        cnl::wide_integer<16, unsigned>{0x12},
                        cnl::_impl::scale<-8, 2, cnl::wide_integer<16, unsigned>>(0x1234)),
                "cnl::_impl::scale test failed");

        static_assert(
                identical(
                        cnl::wide_integer<16, cnl::int8>{0x12},
                        cnl::scale<-8, 2, cnl::wide_integer<16, cnl::uint8>>{}(0x1234)),
                "cnl::_impl::scale test failed");
        static_assert(
                identical(
                        cnl::wide_integer<16, cnl::int8>{0x12},
                        cnl::_impl::scale<-8, 2, cnl::wide_integer<16, cnl::uint8>>(0x1234)),
                "cnl::_impl::scale test failed");
        static_assert(
                identical(
                        cnl::wide_integer<16, cnl::int8>{-0x12},
                        cnl::_impl::scale<-8, 2, cnl::wide_integer<16, cnl::int8>>(-0x1234)),
                "cnl::_impl::scale test failed");
        static_assert(
                identical(
                        cnl::wide_integer<16, unsigned>{0x12},
                        cnl::_impl::scale<-8, 2, cnl::wide_integer<16, unsigned>>(0x1234)),
                "cnl::_impl::scale test failed");

        static_assert(
                identical(
                        cnl::wide_integer<63>{0x55},
                        cnl::scale<-1, 2, cnl::wide_integer<63>>{}(0xAA)),
                "cnl::scale<cnl::wide_integer>");
#if !defined(_MSC_VER)
        static_assert(
                identical(
                        cnl::wide_integer<255>{0x55},
                        cnl::scale<-1, 2, cnl::wide_integer<255>>{}(0xAA)),
                "cnl::scale<cnl::wide_integer>");
        static_assert(
                identical(
                        cnl::wide_integer<64>{1} << 31,
                        cnl::scale<31, 2, cnl::wide_integer<64>>{}(1)),
                "cnl::scale<cnl::wide_integer>");
#endif
    }

    namespace test_fixed_width_scale {
        static_assert(
                identical(
                        cnl::wide_integer<40>{38725387LL << 10},
                        cnl::fixed_width_scale<10, 2, cnl::wide_integer<40>>{}(38725387)),
                "cnl::scale<cnl::wide_integer>");
    }
}
