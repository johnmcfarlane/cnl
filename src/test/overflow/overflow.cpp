
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/overflow.h>
#include <cnl/_impl/type_traits/identical.h>

#include <gtest/gtest.h>

namespace {
    using cnl::_impl::identical;

    namespace test_native_overflow {
        using cnl::native_overflow;

        // convert
        static_assert(identical(cnl::_impl::convert<cnl::native_overflow_tag, cnl::uint8>(259), cnl::uint8{3}),
                "cnl::convert test failed");
        static_assert(identical(cnl::_impl::convert<cnl::native_overflow_tag, cnl::uint16>(-123), cnl::uint16{65413}),
                "cnl::convert test failed");
        static_assert(identical(cnl::_impl::convert<cnl::native_overflow_tag, cnl::int32>(55), 55),
                "cnl::convert test failed");

        // add
        static_assert(identical(add(native_overflow, UINT32_C(0xFFFFFFFF), UINT32_C(0x12345678)), UINT32_C(0xFFFFFFFF)+UINT32_C(0x12345678)), "cnl::add test failed");

        // subtract
        static_assert(identical(
                cnl::_impl::tagged_binary_operator<cnl::native_overflow_tag, cnl::_impl::subtract_op>()(INT8_C(0), INT8_C(0)),
                0), "cnl::subtract test failed");
        static_assert(identical(subtract(native_overflow, INT8_C(0), INT8_C(0)), 0), "cnl::subtract test failed");

        // multiply
        static_assert(identical(multiply(native_overflow, UINT16_C(576), INT32_C(22)), decltype(UINT16_C(576)*INT32_C(22)){12672}), "cnl::multiply test failed");
    }

    namespace test_throwing_overflow {
        using cnl::throwing_overflow;

        // subtract
        static_assert(identical(subtract(throwing_overflow, INT8_C(0), INT8_C(0)), 0), "cnl::add test failed");

        // multiply
#if ! defined(__APPLE__) || ! defined(__clang__)
        static_assert(identical(multiply(throwing_overflow, UINT16_C(576), INT32_C(22)), decltype(UINT16_C(576)*INT32_C(22)){12672}), "cnl::add test failed");
#endif
    }

    namespace test_saturated {
        using cnl::saturated_overflow;

        // convert
        static_assert(identical(cnl::_impl::convert<cnl::saturated_overflow_tag, cnl::uint8>(259), cnl::uint8{255}),
                "cnl::convert test failed");
        static_assert(identical(cnl::_impl::convert<cnl::saturated_overflow_tag, cnl::uint16>(-123), cnl::uint16{0}),
                "cnl::convert test failed");
        static_assert(identical(cnl::_impl::convert<cnl::saturated_overflow_tag, cnl::int32>(55), 55),
                "cnl::convert test failed");

        // add
        static_assert(identical(
                cnl::_impl::tagged_binary_operator<cnl::saturated_overflow_tag, cnl::_impl::add_op>()(7, 23U),
                7+23U), "");
        static_assert(identical(
                std::numeric_limits<decltype(UINT32_C(0xFFFFFFFF)+INT32_C(0x12345678))>::max(),
                add(cnl::saturated_overflow, UINT32_C(0xFFFFFFFF), INT32_C(0x12345678))), "cnl::add test failed");
        static_assert(identical(cnl::numeric_limits<decltype(2 + 2U)>::max(), add(saturated_overflow, 2, cnl::numeric_limits<unsigned>::max())), "cnl::add test failed");

        // subtract
        static_assert(identical(subtract(saturated_overflow, INT8_C(0), INT8_C(0)), 0), "cnl::subtract test failed");
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4308)
#endif
        static_assert(identical(subtract(saturated_overflow, 0U, -1), 1U), "cnl::subtract test failed");
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

        // multiply
#if ! defined(__APPLE__) || ! defined(__clang__)
        static_assert(identical(multiply(saturated_overflow, UINT16_C(576), INT32_C(22)),
                decltype(UINT16_C(576)*INT32_C(22)){12672}), "cnl::multiply test failed");
#endif
        static_assert(identical(
                multiply(saturated_overflow, cnl::numeric_limits<int32_t>::max(), INT32_C(2)),
                cnl::numeric_limits<int32_t>::max()), "cnl::multiply test failed");

        // compare
        static_assert(identical(
                cnl::_impl::convert<cnl::saturated_overflow_tag, short>(cnl::numeric_limits<double>::max()),
                cnl::numeric_limits<short>::max()), "cnl::convert test failed");
        static_assert(identical(
                cnl::_impl::convert<cnl::saturated_overflow_tag, short>(cnl::numeric_limits<double>::lowest()),
                cnl::numeric_limits<short>::lowest()), "cnl::convert test failed");

        // shift_left
        static_assert(identical(
                cnl::numeric_limits<cnl::int16>::max()<<1,
                cnl::shift_left(saturated_overflow, cnl::numeric_limits<cnl::int16>::max(), 1)),
                "cnl::shift_left test failed");
        static_assert(identical(
                cnl::numeric_limits<cnl::int32>::max(),
                cnl::shift_left(saturated_overflow, cnl::numeric_limits<cnl::int32>::max(), 1)),
                "cnl::shift_left test failed");
        static_assert(identical(
                -2,
                cnl::shift_left(saturated_overflow, -1, 1)),
                "cnl::shift_left test failed");
        static_assert(identical(
                cnl::numeric_limits<int>::max(),
                cnl::_impl::tagged_binary_operator<
                        cnl::saturated_overflow_tag, cnl::_impl::shift_left_op>{}(std::uint8_t{255}, 30u)),
                "");
    }

    namespace test_negative_shift_left {
        static_assert(
                identical(
                        2*-1073741824,
                        cnl::shift_left(cnl::trapping_overflow, -1073741824, 1)),
                "cnl::shift_left with negative input");

#if !defined(CNL_UNREACHABLE_UB_ENABLED)
        TEST(overflow, trap)
        {
            ASSERT_DEATH(
                    cnl::shift_left(cnl::trapping_overflow, -1073741825, 1),
                    "negative overflow");
        }
#endif
    }
}
