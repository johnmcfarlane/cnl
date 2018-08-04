
//          Copyright John McFarlane 2015 - 2016.
// Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file ../../LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// disable warnings which warn against errors for whose avoidance overflow is designed
#if defined(_MSC_VER)
#pragma warning(disable: 4018)
#elif defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif

#include <cnl/overflow.h>

#include <gtest/gtest.h>

namespace {
    using cnl::_impl::identical;

    using cnl::convert;

    namespace test_positive_digits {
        using cnl::_impl::positive_digits;

        static_assert(positive_digits<int8_t>::value==7,
                      "cnl::_impl::positive_digits test failed");
        static_assert(positive_digits<uint8_t>::value==8,
                      "cnl::_impl::positive_digits test failed");
        static_assert(positive_digits<int16_t>::value==15,
                      "cnl::_impl::positive_digits test failed");
        static_assert(positive_digits<uint16_t>::value==16,
                      "cnl::_impl::positive_digits test failed");
        static_assert(positive_digits<int32_t>::value==31,
                      "cnl::_impl::positive_digits test failed");
        static_assert(positive_digits<uint32_t>::value==32,
                      "cnl::_impl::positive_digits test failed");
        static_assert(positive_digits<int64_t>::value==63,
                      "cnl::_impl::positive_digits test failed");
        static_assert(positive_digits<uint64_t>::value==64,
                      "cnl::_impl::positive_digits test failed");
    }
    
    namespace test_negative_digits {
        using cnl::_impl::negative_digits;

        static_assert(negative_digits<int8_t>::value==7,
                      "cnl::_impl::negative_digits test failed");
        static_assert(negative_digits<uint8_t>::value==0,
                      "cnl::_impl::negative_digits test failed");
        static_assert(negative_digits<int16_t>::value==15,
                      "cnl::_impl::negative_digits test failed");
        static_assert(negative_digits<uint16_t>::value==0,
                      "cnl::_impl::negative_digits test failed");
        static_assert(negative_digits<int32_t>::value==31,
                      "cnl::_impl::negative_digits test failed");
        static_assert(negative_digits<uint32_t>::value==0,
                      "cnl::_impl::negative_digits test failed");
        static_assert(negative_digits<int64_t>::value==63,
                      "cnl::_impl::negative_digits test failed");
        static_assert(negative_digits<uint64_t>::value==0,
                      "cnl::_impl::negative_digits test failed");
    }
    
    namespace test_convert {
        template<typename Destination, typename Source>
        using convert_test = cnl::_impl::convert_test<Destination, Source>;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::convert_test<>::positive

        static_assert(!convert_test<uint8_t, int8_t>::positive(-1),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(!convert_test<uint8_t, int8_t>::positive(127),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(!convert_test<uint8_t, uint16_t>::positive(0),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(!convert_test<uint8_t, uint16_t>::positive(255),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(convert_test<int8_t, uint16_t>::positive(256),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(!convert_test<uint8_t, int16_t>::positive(-1),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(!convert_test<uint8_t, int16_t>::positive(0),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(!convert_test<uint8_t, int16_t>::positive(255),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(convert_test<int8_t, int16_t>::positive(256),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(!convert_test<uint8_t, int16_t>::positive(-1),
                "cnl::_impl::convert_test<>::positive test failed");

        static_assert(!convert_test<int8_t, uint8_t>::positive(0),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(!convert_test<int8_t, uint8_t>::positive(127),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(convert_test<int8_t, uint8_t>::positive(128),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(!convert_test<int8_t, uint16_t>::positive(0),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(!convert_test<int8_t, uint16_t>::positive(127),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(convert_test<int8_t, uint16_t>::positive(128),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(!convert_test<int8_t, int16_t>::positive(-1),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(!convert_test<int8_t, int16_t>::positive(0),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(!convert_test<int8_t, int16_t>::positive(127),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(convert_test<int8_t, int16_t>::positive(128),
                "cnl::_impl::convert_test<>::positive test failed");

        static_assert(!convert_test<uint16_t, uint8_t>::positive(0),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(!convert_test<uint16_t, uint8_t>::positive(255),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(!convert_test<uint16_t, int8_t>::positive(-128),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(!convert_test<uint16_t, int8_t>::positive(127),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(!convert_test<uint16_t, int16_t>::positive(-32768),
                "cnl::_impl::convert_test<>::positive test failed");
        static_assert(!convert_test<uint16_t, int16_t>::positive(32767),
                "cnl::_impl::convert_test<>::positive test failed");

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::convert_test<>::negative

        static_assert(convert_test<uint8_t, int8_t>::negative(-1),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(!convert_test<uint8_t, int8_t>::negative(127),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(!convert_test<uint8_t, uint16_t>::negative(0),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(!convert_test<uint8_t, uint16_t>::negative(255),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(!convert_test<uint8_t, uint16_t>::negative(256),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(convert_test<uint8_t, int16_t>::negative(-1),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(!convert_test<uint8_t, int16_t>::negative(0),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(!convert_test<uint8_t, int16_t>::negative(255),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(!convert_test<uint8_t, int16_t>::negative(256),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(convert_test<uint8_t, int16_t>::negative(-1),
                "cnl::_impl::convert_test<>::negative test failed");

        static_assert(!convert_test<int8_t, uint8_t>::negative(0),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(!convert_test<int8_t, uint8_t>::negative(127),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(!convert_test<int8_t, uint8_t>::negative(128),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(!convert_test<int8_t, uint16_t>::negative(0),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(!convert_test<int8_t, uint16_t>::negative(127),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(!convert_test<int8_t, uint16_t>::negative(128),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(convert_test<int8_t, int16_t>::negative(-129),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(!convert_test<int8_t, int16_t>::negative(-128),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(!convert_test<int8_t, int16_t>::negative(0),
                "cnl::_impl::convert_test<>::negative test failed");

        static_assert(!convert_test<uint16_t, uint8_t>::negative(0),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(!convert_test<uint16_t, uint8_t>::negative(255),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(convert_test<uint16_t, int8_t>::negative(-1),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(!convert_test<uint16_t, int8_t>::negative(0),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(!convert_test<uint16_t, int8_t>::negative(127),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(convert_test<uint16_t, int16_t>::negative(-32768),
                "cnl::_impl::convert_test<>::negative test failed");
        static_assert(!convert_test<uint16_t, int16_t>::negative(32767),
                "cnl::_impl::convert_test<>::negative test failed");
    }

    namespace test_minus {
        template<typename Rhs>
        using minus_test = cnl::_impl::overflow_test<cnl::_impl::minus_op, Rhs>;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::overflow_test<cnl::_impl::minus_op>::positive

        static_assert(!minus_test<int>::positive(INT_MAX),
                      "minus_test<int>::positive");
        static_assert(!minus_test<int>::positive(-INT_MAX),
                      "minus_test<int>::positive");
        static_assert(minus_test<int>::positive(INT_MIN),
                      "minus_test<int>::positive");

        static_assert(!minus_test<unsigned>::positive(0),
                      "minus_test<unsigned>::positive");
        static_assert(!minus_test<unsigned>::positive(1),
                      "minus_test<unsigned>::positive");
        static_assert(!minus_test<unsigned>::positive(UINT_MAX),
                      "minus_test<unsigned>::positive");

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::overflow_test<cnl::_impl::minus_op>::negative

        static_assert(!minus_test<int>::negative(INT_MAX),
                      "minus_test<int>::negative");
        static_assert(!minus_test<int>::negative(-INT_MAX),
                      "minus_test<int>::negative");
        static_assert(!minus_test<int>::negative(INT_MIN),
                      "minus_test<int>::negative");

        static_assert(!minus_test<unsigned>::negative(0),
                      "minus_test<unsigned>::negative");
        static_assert(minus_test<unsigned>::negative(1),
                      "minus_test<unsigned>::negative");
        static_assert(minus_test<unsigned>::negative(UINT_MAX),
                      "minus_test<unsigned>::negative");
    }

    namespace test_add {
        template<typename Lhs, typename Rhs>
        using add_test = cnl::_impl::overflow_test<cnl::_impl::add_op, Lhs, Rhs>;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::overflow_test<cnl::_impl::add_op>::positive

        static_assert(add_test<int, int>::positive(2000000000, 2000000000),
                "add_test<int, int>::positive");
        static_assert(!add_test<int, int>::positive(-2000000000, 2000000000),
                "add_test<int, int>::positive");
        static_assert(!add_test<int, int>::positive(2000000000, -2000000000),
                "add_test<int, int>::positive");
        static_assert(!add_test<int, int>::positive(-2000000000, -2000000000),
                "add_test<int, int>::positive");

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::overflow_test<cnl::_impl::add_op>::negative

        static_assert(!add_test<int, int>::negative(2000000000, 2000000000),
                "add_test<int, int>::negative");
        static_assert(!add_test<int, int>::negative(-2000000000, 2000000000),
                "add_test<int, int>::negative");
        static_assert(!add_test<int, int>::negative(2000000000, -2000000000),
                "add_test<int, int>::negative");
        static_assert(add_test<int, int>::negative(-2000000000, -2000000000),
                "add_test<int, int>::negative");
    }

    namespace test_subtract {
        template<typename Lhs, typename Rhs>
        using subtract_test = cnl::_impl::overflow_test<cnl::_impl::subtract_op, Lhs, Rhs>;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::overflow_test<cnl::_impl::subtract_op>::positive

        static_assert(!subtract_test<int, int>::positive(2000000000, 2000000000),
                "subtract_test<int, int>::positive");
        static_assert(!subtract_test<int, int>::positive(-2000000000, 2000000000),
                "subtract_test<int, int>::positive");
        static_assert(subtract_test<int, int>::positive(2000000000, -2000000000),
                "subtract_test<int, int>::positive");
        static_assert(!subtract_test<int, int>::positive(-2000000000, -2000000000),
                "subtract_test<int, int>::positive");

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::overflow_test<cnl::_impl::subtract_op>::negative

        static_assert(!subtract_test<int, int>::negative(2000000000, 2000000000),
                "subtract_test<int, int>::negative");
        static_assert(subtract_test<int, int>::negative(-2000000000, 2000000000),
                "subtract_test<int, int>::negative");
        static_assert(!subtract_test<int, int>::negative(2000000000, -2000000000),
                "subtract_test<int, int>::negative");
        static_assert(!subtract_test<int, int>::negative(-2000000000, -2000000000),
                "subtract_test<int, int>::negative");

        static_assert(subtract_test<unsigned, unsigned>::negative(0, 1),
                "subtract_test<int, int>::negative");
    }

    namespace test_multiply {
        template<typename Lhs, typename Rhs>
        using multiply_test = cnl::_impl::overflow_test<cnl::_impl::multiply_op, Lhs, Rhs>;

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::overflow_test<cnl::_impl::multiply_op>::positive

        static_assert(multiply_test<int, int>::positive(2000000000, 2),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(21, 2),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-2000000000, 2),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-21, 2),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(2000000000, -2),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(21, -2),
                "multiply_test<int, int>::positive");
        static_assert(multiply_test<int, int>::positive(-2000000000, -2),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-21, -2),
                "multiply_test<int, int>::positive");

        static_assert(multiply_test<int, int>::positive(2, 2000000000),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(2, 21),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(2, -2000000000),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(2, -21),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-2, 2000000000),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-2, 21),
                "multiply_test<int, int>::positive");
        static_assert(multiply_test<int, int>::positive(-2, -2000000000),
                "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-2, -21),
                "multiply_test<int, int>::positive");

        // boundary
        static_assert(multiply_test<int, int>::positive(-46341, -46341),
                      "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-46341, -46340),
                      "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-46341, 46340),
                      "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-46341, 46341),
                      "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-46340, -46340),
                      "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-46340, 46340),
                      "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(-46340, 46341),
                      "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(46340, 46340),
                      "multiply_test<int, int>::positive");
        static_assert(!multiply_test<int, int>::positive(46340, 46341),
                      "multiply_test<int, int>::positive");
        static_assert(multiply_test<int, int>::positive(46341, 46341),
                      "multiply_test<int, int>::positive");

        ////////////////////////////////////////////////////////////////////////////////
        // cnl::_impl::overflow_test<cnl::_impl::multiply_op>::negative

        static_assert(!multiply_test<int, int>::negative(2000000000, 2),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(21, 2),
                "multiply_test<int, int>::negative");
        static_assert(multiply_test<int, int>::negative(-2000000000, 2),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-21, 2),
                "multiply_test<int, int>::negative");
        static_assert(multiply_test<int, int>::negative(2000000000, -2),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(21, -2),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-2000000000, -2),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-21, -2),
                "multiply_test<int, int>::negative");

        static_assert(!multiply_test<int, int>::negative(2, 2000000000),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(2, 21),
                "multiply_test<int, int>::negative");
        static_assert(multiply_test<int, int>::negative(2, -2000000000),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(2, -21),
                "multiply_test<int, int>::negative");
        static_assert(multiply_test<int, int>::negative(-2, 2000000000),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-2, 21),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-2, -2000000000),
                "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-2, -21),
                "multiply_test<int, int>::negative");

        // boundary
        static_assert(!multiply_test<int, int>::negative(-46341, -46341),
                      "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-46341, -46340),
                      "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-46341, 46340),
                      "multiply_test<int, int>::negative");
        static_assert(multiply_test<int, int>::negative(-46341, 46341),
                      "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-46340, -46340),
                      "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-46340, 46340),
                      "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(-46340, 46341),
                      "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(46340, 46340),
                      "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(46340, 46341),
                      "multiply_test<int, int>::negative");
        static_assert(!multiply_test<int, int>::negative(46341, 46341),
                      "multiply_test<int, int>::negative");

        ////////////////////////////////////////////////////////////////////////////////
        // most negative number

        static_assert(multiply_test<int, int>::positive(INT_MIN, -1), "multiply most negative number");
        static_assert(!multiply_test<int, int>::negative(INT_MIN, -1), "multiply most negative number");
    }

    namespace test_divide {
        template<typename Lhs, typename Rhs>
        using divide_test = cnl::_impl::overflow_test<cnl::_impl::divide_op, Lhs, Rhs>;

        ////////////////////////////////////////////////////////////////////////////////
        // most negative number

        static_assert(cnl::_impl::has_most_negative_number<int>::value, "");
        static_assert(divide_test<int, int>::positive(INT_MIN, -1), "divide most negative number");
        static_assert(!divide_test<int, int>::negative(INT_MIN, -1), "divide most negative number");
    }

    namespace test_native_overflow {
        using cnl::native_overflow;

        // convert
        static_assert(identical(convert<cnl::native_overflow_tag, cnl::uint8>{}(259), cnl::uint8{3}),
                "cnl::convert test failed");
        static_assert(identical(convert<cnl::native_overflow_tag, cnl::uint16>{}(-123), cnl::uint16{65413}),
                "cnl::convert test failed");
        static_assert(identical(convert<cnl::native_overflow_tag, cnl::int32>{}(55), 55),
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
        static_assert(identical(convert<cnl::saturated_overflow_tag, cnl::uint8>{}(259), cnl::uint8{255}),
                "cnl::convert test failed");
        static_assert(identical(convert<cnl::saturated_overflow_tag, cnl::uint16>{}(-123), cnl::uint16{0}),
                "cnl::convert test failed");
        static_assert(identical(convert<cnl::saturated_overflow_tag, cnl::int32>{}(55), 55),
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
                convert<cnl::saturated_overflow_tag, short>{}(cnl::numeric_limits<double>::max()),
                cnl::numeric_limits<short>::max()), "cnl::convert test failed");

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
    }

    namespace test_negative_shift_left {
        static_assert(
                identical(
                        2*-1073741824,
                        cnl::shift_left(cnl::trapping_overflow, -1073741824, 1)),
                "cnl::shift_left with negative input");

        TEST(overflow, trap)
        {
            ASSERT_DEATH(
                    cnl::shift_left(cnl::trapping_overflow, -1073741825, 1),
                    "negative overflow");
        }
    }
}
