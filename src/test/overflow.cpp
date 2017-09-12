
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

// TODO: remove ASAP
#if defined(_MSC_VER)
#pragma warning(disable: 4307)
#endif

namespace {
    using cnl::_impl::identical;

    using cnl::convert;
    using cnl::add;
    using cnl::subtract;
    using cnl::multiply;

    namespace test_impl {
        using namespace cnl::_overflow_impl;
        
        ////////////////////////////////////////////////////////////////////////////////
        // positive_digits

        static_assert(positive_digits<int8_t>::value==7,
                "positive_digits test failed");
        static_assert(positive_digits<uint8_t>::value==8,
                "positive_digits test failed");
        static_assert(positive_digits<int16_t>::value==15,
                "positive_digits test failed");
        static_assert(positive_digits<uint16_t>::value==16,
                "positive_digits test failed");
        static_assert(positive_digits<int32_t>::value==31,
                "positive_digits test failed");
        static_assert(positive_digits<uint32_t>::value==32,
                "positive_digits test failed");
        static_assert(positive_digits<int64_t>::value==63,
                "positive_digits test failed");
        static_assert(positive_digits<uint64_t>::value==64,
                "positive_digits test failed");

        ////////////////////////////////////////////////////////////////////////////////
        // negative_digits

        static_assert(negative_digits<int8_t>::value==7,
                "negative_digits test failed");
        static_assert(negative_digits<uint8_t>::value==0,
                "negative_digits test failed");
        static_assert(negative_digits<int16_t>::value==15,
                "negative_digits test failed");
        static_assert(negative_digits<uint16_t>::value==0,
                "negative_digits test failed");
        static_assert(negative_digits<int32_t>::value==31,
                "negative_digits test failed");
        static_assert(negative_digits<uint32_t>::value==0,
                "negative_digits test failed");
        static_assert(negative_digits<int64_t>::value==63,
                "negative_digits test failed");
        static_assert(negative_digits<uint64_t>::value==0,
                "negative_digits test failed");

        ////////////////////////////////////////////////////////////////////////////////
        // is_positive_overflow

        static_assert(!is_positive_overflow<uint8_t, int8_t>(-1),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint8_t, int8_t>(127),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint8_t, uint16_t>(0),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint8_t, uint16_t>(255),
                "is_positive_overflow test failed");
        static_assert(is_positive_overflow<uint8_t, uint16_t>(256),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint8_t, int16_t>(-1),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint8_t, int16_t>(0),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint8_t, int16_t>(255),
                "is_positive_overflow test failed");
        static_assert(is_positive_overflow<uint8_t, int16_t>(256),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint8_t, int16_t>(-1),
                "is_positive_overflow test failed");

        static_assert(!is_positive_overflow<int8_t, uint8_t>(0),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<int8_t, uint8_t>(127),
                "is_positive_overflow test failed");
        static_assert(is_positive_overflow<int8_t, uint8_t>(128),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<int8_t, uint16_t>(0),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<int8_t, uint16_t>(127),
                "is_positive_overflow test failed");
        static_assert(is_positive_overflow<int8_t, uint16_t>(128),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<int8_t, int16_t>(-1),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<int8_t, int16_t>(0),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<int8_t, int16_t>(127),
                "is_positive_overflow test failed");
        static_assert(is_positive_overflow<int8_t, int16_t>(128),
                "is_positive_overflow test failed");

        static_assert(!is_positive_overflow<uint16_t, uint8_t>(0),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint16_t, uint8_t>(255),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint16_t, int8_t>(-128),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint16_t, int8_t>(127),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint16_t, int16_t>(-32768),
                "is_positive_overflow test failed");
        static_assert(!is_positive_overflow<uint16_t, int16_t>(32767),
                "is_positive_overflow test failed");

        ////////////////////////////////////////////////////////////////////////////////
        // is_negative_overflow

        static_assert(is_negative_overflow<uint8_t, int8_t>(-1),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint8_t, int8_t>(127),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint8_t, uint16_t>(0),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint8_t, uint16_t>(255),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint8_t, uint16_t>(256),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(is_negative_overflow<uint8_t, int16_t>(-1),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint8_t, int16_t>(0),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint8_t, int16_t>(255),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint8_t, int16_t>(256),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(is_negative_overflow<uint8_t, int16_t>(-1),
                "cnl::_integer_impl::is_negative_overflow test failed");

        static_assert(!is_negative_overflow<int8_t, uint8_t>(0),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<int8_t, uint8_t>(127),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<int8_t, uint8_t>(128),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<int8_t, uint16_t>(0),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<int8_t, uint16_t>(127),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<int8_t, uint16_t>(128),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(is_negative_overflow<int8_t, int16_t>(-129),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<int8_t, int16_t>(-128),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<int8_t, int16_t>(0),
                "cnl::_integer_impl::is_negative_overflow test failed");

        static_assert(!is_negative_overflow<uint16_t, uint8_t>(0),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint16_t, uint8_t>(255),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(is_negative_overflow<uint16_t, int8_t>(-1),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint16_t, int8_t>(0),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint16_t, int8_t>(127),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(is_negative_overflow<uint16_t, int16_t>(-32768),
                "cnl::_integer_impl::is_negative_overflow test failed");
        static_assert(!is_negative_overflow<uint16_t, int16_t>(32767),
                "cnl::_integer_impl::is_negative_overflow test failed");

        static_assert(cnl::_overflow_impl::is_multiply_overflow(2000000000, 2), "safe_elastic_integer operator*");
        static_assert(!cnl::_overflow_impl::is_multiply_overflow(21, 2), "safe_elastic_integer operator*");
        static_assert(cnl::_overflow_impl::is_multiply_overflow(-2000000000, 2), "safe_elastic_integer operator*");
        static_assert(!cnl::_overflow_impl::is_multiply_overflow(-21, 2), "safe_elastic_integer operator*");
        static_assert(cnl::_overflow_impl::is_multiply_overflow(2000000000, -2), "safe_elastic_integer operator*");
        static_assert(!cnl::_overflow_impl::is_multiply_overflow(21, -2), "safe_elastic_integer operator*");
        static_assert(cnl::_overflow_impl::is_multiply_overflow(-2000000000, -2), "safe_elastic_integer operator*");
        static_assert(!cnl::_overflow_impl::is_multiply_overflow(-21, -2), "safe_elastic_integer operator*");

        static_assert(cnl::_overflow_impl::is_multiply_overflow(2, 2000000000), "safe_elastic_integer operator*");
        static_assert(!cnl::_overflow_impl::is_multiply_overflow(2, 21), "safe_elastic_integer operator*");
        static_assert(cnl::_overflow_impl::is_multiply_overflow(2, -2000000000), "safe_elastic_integer operator*");
        static_assert(!cnl::_overflow_impl::is_multiply_overflow(2, -21), "safe_elastic_integer operator*");
        static_assert(cnl::_overflow_impl::is_multiply_overflow(-2, 2000000000), "safe_elastic_integer operator*");
        static_assert(!cnl::_overflow_impl::is_multiply_overflow(-2, 21), "safe_elastic_integer operator*");
        static_assert(cnl::_overflow_impl::is_multiply_overflow(-2, -2000000000), "safe_elastic_integer operator*");
        static_assert(!cnl::_overflow_impl::is_multiply_overflow(-2, -21), "safe_elastic_integer operator*");
    }

    namespace test_native_overflow {
        using cnl::native_overflow;

        // convert
        static_assert(identical(convert<std::uint8_t>(native_overflow, 259), std::uint8_t{3}), "cnl::convert test failed");
        static_assert(identical(convert<std::uint16_t>(native_overflow, -123), std::uint16_t{65413}), "cnl::convert test failed");
        static_assert(identical(convert<std::int32_t>(native_overflow, std::uint8_t{55}), 55), "cnl::convert test failed");

        // add
        static_assert(identical(add(native_overflow, UINT32_C(0xFFFFFFFF), UINT32_C(0x12345678)), UINT32_C(0xFFFFFFFF)+UINT32_C(0x12345678)), "cnl::add test failed");

        // subtract
        static_assert(identical(cnl::_overflow_impl::operate<cnl::native_overflow_tag, cnl::_impl::subtract_op>()(INT8_C(0), INT8_C(0)), 0), "cnl::subtract test failed");
        static_assert(identical(subtract(native_overflow, INT8_C(0), INT8_C(0)), 0), "cnl::subtract test failed");

        // multiply
        static_assert(identical(multiply(native_overflow, UINT16_C(576), INT32_C(22)), decltype(UINT16_C(576)*INT32_C(22)){12672}), "cnl::multiply test failed");

        // compare
        static_assert(cnl::_overflow_impl::operate<cnl::native_overflow_tag, cnl::_impl::less_than_op>()(-1, 1u) == (-1 < 1u), "cnl::_overflow_impl::operate test failed");
    }

    namespace test_throwing_overflow {
        using cnl::throwing_overflow;

        // subtract
        static_assert(identical(subtract(throwing_overflow, INT8_C(0), INT8_C(0)), 0), "cnl::add test failed");

        // multiply
        static_assert(identical(multiply(throwing_overflow, UINT16_C(576), INT32_C(22)), decltype(UINT16_C(576)*INT32_C(22)){12672}), "cnl::add test failed");
    }

    namespace test_saturated {
        using cnl::saturated_overflow;

        // convert
        static_assert(identical(convert<std::uint8_t>(saturated_overflow, 259), std::uint8_t{255}), "cnl::convert test failed");
        static_assert(identical(convert<std::uint16_t>(saturated_overflow, -123), std::uint16_t{0}), "cnl::convert test failed");
        static_assert(identical(convert<std::int32_t>(saturated_overflow, std::uint8_t{55}), 55), "cnl::convert test failed");

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
        static_assert(identical(multiply(saturated_overflow, UINT16_C(576), INT32_C(22)),
                decltype(UINT16_C(576)*INT32_C(22)){12672}), "cnl::multiply test failed");
        static_assert(identical(
                multiply(saturated_overflow, cnl::numeric_limits<int32_t>::max(), INT32_C(2)),
                cnl::numeric_limits<int32_t>::max()), "cnl::multiply test failed");

        // compare
        static_assert(cnl::_overflow_impl::operate<cnl::saturated_overflow_tag, cnl::_impl::less_than_op>()(-1, 1u), "cnl::_overflow_impl::operate test failed");
        static_assert(identical(convert<short>(saturated_overflow, cnl::numeric_limits<double>::max()),
                                cnl::numeric_limits<short>::max()), "cnl::convert test failed");
    }
}
