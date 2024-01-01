
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <test.h>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4308)
#endif
#include <cnl/overflow.h>
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include <limits>

namespace {
    namespace test_native_overflow {

        // convert
        static_assert(identical(
                std::uint8_t{3},
                cnl::convert<cnl::native_overflow_tag, std::uint8_t>{}(259)));
        static_assert(identical(
                std::uint16_t{65413},
                cnl::convert<cnl::native_overflow_tag, std::uint16_t>{}(-123)));
        static_assert(identical(
                55,
                cnl::convert<cnl::native_overflow_tag, std::int32_t>{}(55)));

        // add
        static_assert(
                identical(
                        cnl::_impl::operate<cnl::_impl::add_op, cnl::native_overflow_tag>{}(
                                0xFFFFFFFFU, 0x12345678U),
                        0xFFFFFFFFU + 0x12345678U));

        // subtract
        static_assert(
                identical(
                        cnl::custom_operator<
                                cnl::_impl::subtract_op,
                                cnl::op_value<std::int8_t, cnl::native_overflow_tag>,
                                cnl::op_value<std::int8_t, cnl::native_overflow_tag>>()(
                                std::int8_t{40}, std::int8_t{-2}),
                        std::int8_t{40} - std::int8_t{-2}));
        static_assert(identical(
                cnl::_impl::operate<cnl::_impl::subtract_op, cnl::native_overflow_tag>{}(
                        std::int8_t{-40}, std::int8_t{-82}),
                std::int8_t{-40} - std::int8_t{-82}));

        // multiply
        static_assert(identical(
                cnl::_impl::operate<cnl::_impl::multiply_op, cnl::native_overflow_tag>{}(
                        std::uint16_t{576}, std::int32_t{22}),
                std::uint16_t{576} * std::int32_t{22}));
    }

    namespace test_throwing_overflow {
        // subtract
        static_assert(identical(
                cnl::_impl::operate<cnl::_impl::subtract_op, cnl::_impl::throwing_overflow_tag>{}(
                        std::int8_t{40}, std::int8_t{-2}),
                std::int8_t{40} - std::int8_t{-2}));

        // multiply
        static_assert(identical(
                cnl::_impl::operate<cnl::_impl::multiply_op, cnl::_impl::throwing_overflow_tag>{}(
                        std::uint16_t{576}, std::int32_t{22}),
                std::uint16_t{576} * std::int32_t{22}));
    }

    namespace test_saturated {
        // convert
        static_assert(identical(
                std::uint8_t{255},
                cnl::convert<cnl::saturated_overflow_tag, std::uint8_t>{}(259)));
        static_assert(identical(
                std::uint16_t{0},
                cnl::convert<cnl::saturated_overflow_tag, std::uint16_t>{}(-123)));
        static_assert(identical(
                55,
                cnl::convert<cnl::saturated_overflow_tag, std::int32_t>{}(55)));

        // add
        static_assert(
                identical(
                        cnl::custom_operator<
                                cnl::_impl::add_op,
                                cnl::op_value<signed, cnl::saturated_overflow_tag>,
                                cnl::op_value<unsigned, cnl::saturated_overflow_tag>>()(7, 23U),
                        7 + 23U));
        static_assert(identical(
                std::numeric_limits<
                        decltype(std::uint32_t{0xFFFFFFFF} + std::int32_t{0x12345678})>::max(),
                cnl::_impl::operate<cnl::_impl::add_op, cnl::saturated_overflow_tag>{}(
                        std::uint32_t{0xFFFFFFFF}, std::int32_t{0x12345678})));
        static_assert(
                identical(
                        std::numeric_limits<decltype(2 + 2U)>::max(),
                        cnl::_impl::operate<cnl::_impl::add_op, cnl::saturated_overflow_tag>{}(
                                2, std::numeric_limits<unsigned>::max())));

        // subtract
        static_assert(identical(
                cnl::_impl::operate<cnl::_impl::subtract_op, cnl::saturated_overflow_tag>{}(
                        std::int8_t{40}, std::int8_t{-2}),
                std::int8_t{40} - std::int8_t{-2}));
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4308)
#endif
        static_assert(identical(
                cnl::_impl::operate<cnl::_impl::subtract_op, cnl::saturated_overflow_tag>{}(0U, -1),
                0U - -1));
#if defined(_MSC_VER)
#pragma warning(pop)
#endif
        static_assert(
                identical(
                        std::numeric_limits<int>::max(),
                        cnl::_impl::operate<cnl::_impl::subtract_op, cnl::saturated_overflow_tag>{}(
                                0, std::numeric_limits<int>::min())));

        // multiply
        static_assert(identical(
                cnl::_impl::operate<cnl::_impl::multiply_op, cnl::saturated_overflow_tag>{}(
                        std::uint16_t{576}, std::int32_t{22}),
                std::uint16_t{576} * std::int32_t{22}));
        static_assert(identical(
                cnl::_impl::operate<cnl::_impl::multiply_op, cnl::saturated_overflow_tag>{}(
                        std::numeric_limits<int32_t>::max(), std::int32_t{2}),
                std::numeric_limits<int32_t>::max()));

        // convert
        static_assert(identical(
                std::numeric_limits<short>::max(),
                cnl::convert<cnl::saturated_overflow_tag, short>{}(
                        std::numeric_limits<double>::max())));
        static_assert(identical(
                std::numeric_limits<short>::lowest(),
                cnl::convert<cnl::saturated_overflow_tag, short>{}(
                        std::numeric_limits<double>::lowest())));

        // shift_left
        static_assert(
                identical(
                        std::numeric_limits<std::int16_t>::max() << 1,
                        cnl::_impl::operate<cnl::_impl::shift_left_op, cnl::saturated_overflow_tag>{}(
                                std::numeric_limits<std::int16_t>::max(), 1)));
        static_assert(
                identical(
                        std::numeric_limits<std::int32_t>::max(),
                        cnl::_impl::operate<cnl::_impl::shift_left_op, cnl::saturated_overflow_tag>{}(
                                std::numeric_limits<std::int32_t>::max(), 1)));
        static_assert(
                identical(
                        std::numeric_limits<int>::max(),
                        cnl::custom_operator<
                                cnl::_impl::shift_left_op,
                                cnl::op_value<std::uint8_t, cnl::saturated_overflow_tag>,
                                cnl::op_value<unsigned, cnl::saturated_overflow_tag>>{}(
                                std::uint8_t{255}, 30U)));
    }

    namespace test_negative_shift_left {
#if defined(CNL_DEBUG)
        TEST(overflow, trap)  // NOLINT
        {
            constexpr auto fn{cnl::_impl::operate<cnl::_impl::shift_left_op, cnl::trapping_overflow_tag>{}};
            ASSERT_DEATH(  // NOLINT
                    (void)fn(-1073741825, 1),
                    "negative overflow");
        }
#endif
    }
}
