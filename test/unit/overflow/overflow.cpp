
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cnl/_impl/type_traits/identical.h>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4308)
#endif
#include <cnl/overflow.h>
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include <gtest/gtest.h>

namespace {
    using cnl::_impl::identical;

    namespace test_native_overflow {

        // convert
        static_assert(identical(
                cnl::uint8{3},
                cnl::convert<cnl::native_overflow_tag, cnl::uint8>{}(259)));
        static_assert(identical(
                cnl::uint16{65413},
                cnl::convert<cnl::native_overflow_tag, cnl::uint16>{}(-123)));
        static_assert(identical(
                55,
                cnl::convert<cnl::native_overflow_tag, cnl::int32>{}(55)));

        // add
        static_assert(
                identical(
                        cnl::_impl::operate<cnl::_impl::add_op, cnl::native_overflow_tag>{}(
                                cnl::uint32(0xFFFFFFFF), cnl::uint32(0x12345678)),
                        cnl::uint32(0xFFFFFFFF) + cnl::uint32(0x12345678)));

        // subtract
        static_assert(
                identical(
                        cnl::custom_operator<
                                cnl::_impl::subtract_op,
                                cnl::op_value<cnl::int8, cnl::native_overflow_tag>,
                                cnl::op_value<cnl::int8, cnl::native_overflow_tag>>()(
                                cnl::int8{40}, cnl::int8{-2}),
                        cnl::int8{40} - cnl::int8{-2}));
        static_assert(identical(
                cnl::_impl::operate<cnl::_impl::subtract_op, cnl::native_overflow_tag>{}(
                        cnl::int8{-40}, cnl::int8{-82}),
                cnl::int8{-40} - cnl::int8{-82}));

        // multiply
        static_assert(identical(
                cnl::_impl::operate<cnl::_impl::multiply_op, cnl::native_overflow_tag>{}(
                        cnl::uint16{576}, cnl::int32{22}),
                cnl::uint16{576} * cnl::int32{22}));
    }

    namespace test_throwing_overflow {
        // subtract
        static_assert(identical(
                cnl::_impl::operate<cnl::_impl::subtract_op, cnl::_impl::throwing_overflow_tag>{}(
                        cnl::int8{40}, cnl::int8{-2}),
                cnl::int8{40} - cnl::int8{-2}));

        // multiply
        static_assert(identical(
                cnl::_impl::operate<cnl::_impl::multiply_op, cnl::_impl::throwing_overflow_tag>{}(
                        cnl::uint16{576}, cnl::int32{22}),
                cnl::uint16{576} * cnl::int32{22}));
    }

    namespace test_saturated {
        // convert
        static_assert(identical(
                cnl::uint8{255},
                cnl::convert<cnl::saturated_overflow_tag, cnl::uint8>{}(259)));
        static_assert(identical(
                cnl::uint16{0},
                cnl::convert<cnl::saturated_overflow_tag, cnl::uint16>{}(-123)));
        static_assert(identical(
                55,
                cnl::convert<cnl::saturated_overflow_tag, cnl::int32>{}(55)));

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
                        decltype(cnl::uint32{0xFFFFFFFF} + cnl::int32{0x12345678})>::max(),
                cnl::_impl::operate<cnl::_impl::add_op, cnl::saturated_overflow_tag>{}(
                        cnl::uint32{0xFFFFFFFF}, cnl::int32{0x12345678})));
        static_assert(
                identical(
                        cnl::numeric_limits<decltype(2 + 2U)>::max(),
                        cnl::_impl::operate<cnl::_impl::add_op, cnl::saturated_overflow_tag>{}(
                                2, cnl::numeric_limits<unsigned>::max())));

        // subtract
        static_assert(identical(
                cnl::_impl::operate<cnl::_impl::subtract_op, cnl::saturated_overflow_tag>{}(
                        cnl::int8{40}, cnl::int8{-2}),
                cnl::int8{40} - cnl::int8{-2}));
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
                        cnl::numeric_limits<int>::max(),
                        cnl::_impl::operate<cnl::_impl::subtract_op, cnl::saturated_overflow_tag>{}(
                                0, cnl::numeric_limits<int>::min())));

        // multiply
        static_assert(identical(
                cnl::_impl::operate<cnl::_impl::multiply_op, cnl::saturated_overflow_tag>{}(
                        cnl::uint16{576}, cnl::int32{22}),
                cnl::uint16{576} * cnl::int32{22}));
        static_assert(identical(
                cnl::_impl::operate<cnl::_impl::multiply_op, cnl::saturated_overflow_tag>{}(
                        cnl::numeric_limits<int32_t>::max(), cnl::int32{2}),
                cnl::numeric_limits<int32_t>::max()));

        // convert
        static_assert(identical(
                cnl::numeric_limits<short>::max(),
                cnl::convert<cnl::saturated_overflow_tag, short>{}(
                        cnl::numeric_limits<double>::max())));
        static_assert(identical(
                cnl::numeric_limits<short>::lowest(),
                cnl::convert<cnl::saturated_overflow_tag, short>{}(
                        cnl::numeric_limits<double>::lowest())));

        // shift_left
        static_assert(
                identical(
                        cnl::numeric_limits<cnl::int16>::max() << 1,
                        cnl::_impl::operate<cnl::_impl::shift_left_op, cnl::saturated_overflow_tag>{}(
                                cnl::numeric_limits<cnl::int16>::max(), 1)));
        static_assert(
                identical(
                        cnl::numeric_limits<cnl::int32>::max(),
                        cnl::_impl::operate<cnl::_impl::shift_left_op, cnl::saturated_overflow_tag>{}(
                                cnl::numeric_limits<cnl::int32>::max(), 1)));
        static_assert(
                identical(
                        cnl::numeric_limits<int>::max(),
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
            ASSERT_DEATH(
                    (void)fn(-1073741825, 1),
                    "negative overflow");
        }
#endif
    }
}
